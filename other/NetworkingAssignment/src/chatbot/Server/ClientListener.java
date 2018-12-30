package chatbot.Server;

import chatbot.Files.FileTransferProtocol;
import chatbot.Group;
import chatbot.Message;
import gui_design.ChatRoom;

import java.io.*;
import java.net.Socket;
import javax.swing.*;

/**
 * @author abrsas002
 */
public class ClientListener extends Thread
{

    protected Socket clientSocket;
    Server server;

    InputStream in = null;
    BufferedReader brinp = null;
    DataOutputStream out = null;

    String name;
    int id;

    public ClientListener(Socket clientSocket, Server server)
    {
        this.clientSocket = clientSocket;
        this.server = server;

        try
        {
            in = this.clientSocket.getInputStream();
            brinp = new BufferedReader(new InputStreamReader(in));
            out = new DataOutputStream(this.clientSocket.getOutputStream());
        } catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    public void run()
    {
        while (true)
        {
            //forever read messages from the server
            Message message = receiveFromClient();

            if (message.getCommand().equals("quit"))
            {
                try
                {
                    clientSocket.close();
                    System.out.println("closing clientSocket");
                } catch (IOException ex)
                {
                    ex.printStackTrace();
                }
                return;
            }
            else if (message.getCommand().equals("name"))
            {
                System.out.println("SERVER: found new client conection: '" + message.message + "'");
                name = message.message;//get name from client on connect
                //<editor-fold defaultstate="collapsed" desc="assign this client an id, and inform it">
                id = server.nextClientID - 1;
                sendToThisClient("id::", id + "");
                System.out.println("SERVER found new client connection, given id: " + id);

                //</editor-fold>
                //<editor-fold defaultstate="collapsed" desc="send new client info on all existing clients">
                System.out.println("serer has n clients: " + server.clients.size());
                for (ClientListener client : server.clients.values())
                {
                    System.out.println("client: " + client.name + " " + client.id);
                    sendToThisClient("sndclient", client.id + ":" + client.name);
                }
//</editor-fold>
                //<editor-fold defaultstate="collapsed" desc="inform all existing clients a new client has connected">
                server.sendToAllClients("sndclient::", id + ":" + name);
                //</editor-fold>
            }
            else if (message.getCommand().equals("message"))
            {
                server.sendToClient("message:" + message.getArg(1), message.message, Integer.parseInt(message.getArg(2)));
            }
            else if (message.getCommand().equals("ginfo"))
            {
                String[] group = message.message.split(":");
                int groupID = server.groups.size();
                server.groups.add(new Group(
                        groupID,
                        group,
                        message.getArg(2)));
                // Informing client of group creation
                for (String strID : group)
                {
                    int id = Integer.parseInt(strID);
                    server.sendToClient(message.getCommand() + ":" + groupID + ":" + message.getArg(2), message.message, id);
                }
            }
            else if (message.getCommand().equals("gmessage"))
            {
                int groupID = Integer.parseInt(message.getArg(2));
                Group group = server.groups.get(groupID);

                for (int clientID : group.clients)
                {
                    // don't send to sender
                    if (clientID == Integer.parseInt(message.getArg(1)))
                    {
                        continue;
                    }
                    server.sendToClient("gmessage:" + message.getArg(1) + ":" + groupID, message.message, clientID);
                }
            }
            else if (message.getCommand().equals("amessage"))
            {
                for (Integer otherID : server.clients.keySet())
                {
                    if (otherID == this.id) continue;
                    
                    int key = (this.id + 1) + otherID * 500;
                    String msg = Message.encryptMessage(" BROADCAST " + message.message, key);
                    server.sendToClient("message:" + message.getArg(1) + ":", msg, otherID);
                }

            }
            else if (message.getCommand().equals("closeclient"))
            {
                int leavingClientID = Integer.parseInt(message.message);
                server.clients.remove(leavingClientID);
                server.sendToAllClients("closeclient::", leavingClientID + "");
            }
            else if (message.getCommand().equals("file"))
            {
                // TODO: ask client permission

                if ((JOptionPane.showConfirmDialog(null,
                        "Would you like to recive this file?",
                        "Accepting file?",
                        JOptionPane.YES_NO_OPTION,
                        JOptionPane.QUESTION_MESSAGE)) == JOptionPane.YES_OPTION)
                {
                    try
                    {
                        int receivingClientID = Integer.parseInt(message.getArg(2));

                        File file = new File("file_storage/" + message.message.split(":")[0]);
                        int fileSize = Integer.parseInt(message.message.split(":")[1]);
                        FileTransferProtocol ftp = new FileTransferProtocol();

                        // receive and write file to server
                        if (!ftp.getFile(file, fileSize, in))
                        {
                            return; // tell clients file failed to send
                        }
                        // send file to client
                        server.sendToClient(message.header, message.message, receivingClientID);
                        ftp.sendFile(file, server.clients.get(receivingClientID).out);

                    } catch (Exception e)
                    {
                        e.printStackTrace();
                    }
                }

            }
        }
    }

    public Message receiveFromClient()
    {
        String line;

        try
        {
            line = brinp.readLine() + "\n";
            line += brinp.readLine();

            return new Message(line);
        } catch (IOException e)
        {
            //e.printStackTrace();
            return new Message("q\n\n");
        }
    }

    public void sendToThisClient(String header, String message)
    {
        try
        {
            out.writeChars(header + "\n" + message + "\n");
        } catch (IOException ex)
        {
            //ex.printStackTrace();
        }
    }

}
