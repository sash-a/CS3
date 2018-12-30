package chatbot.Client;

import chatbot.Files.FileTransferProtocol;
import chatbot.Message;
import gui_design.ChatRoom;
import gui_design.GUI;

import java.io.*;

import java.net.Socket;
import java.util.HashMap;

/**
 * @author laptop
 */
public class Client
{

    Client This;

    Socket socket;
    DataInputStream in;
    BufferedReader brinp = null;
    FileOutputStream fileOut;
    BufferedOutputStream brout;

    DataOutputStream out;

    public String name;
    public int id;

    // Maps ID to name
    private Thread serverListener;
    public GUI gui;

    public HashMap<Integer, ChatRoom> privateChatRooms;//id of recipient mapped to the chat between this user and the
    // recipient
    HashMap<Integer, ChatRoom> groupChatRooms;

    public Client(String name, GUI gui)
    {
        this.name = name;
        this.gui = gui;
        privateChatRooms = new HashMap<>();
        groupChatRooms = new HashMap<>();
        This = this;

        try
        {
            socket = new Socket("localhost", 8080);
            in = new DataInputStream(socket.getInputStream());
            brinp = new BufferedReader(new InputStreamReader(in));

            out = new DataOutputStream(socket.getOutputStream());

            sendToServer("name", name);
        } catch (IOException ex)
        {
        }

        serverListener = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                while (true)
                {
                    Message message = receiveFromServer();
                    System.out.println("received message " + message);
                    String command = message.getCommand();
                    if (command.equals("id"))
                    {
                        id = Integer.parseInt(message.message);
                        System.out.println("CLIENT " + This.name + " has been given id " + id);
                    }
                    else if (command.equals("test"))
                    {
                        System.out.println(message);
                    }
                    else if (command.equals("message"))
                    {
                        int sender = Integer.parseInt(message.getArg(1));
                        //System.out.println("message from " + sender + ": " + message.message);
                        if (!privateChatRooms.containsKey(sender))
                        {
                            //if receiver does not have a chat with sender already
                            ChatRoom newChat = new ChatRoom(This, sender, false);
                            newChat.setVisible(true);
                            newChat.setTitle(This.gui.knownClients.get(sender));
                            addChatRoom(newChat);
                        }
                        privateChatRooms.get(sender).recieveMessage(message.message);
                    }
                    else if (command.equals("sndclient"))
                    {
                        This.gui.addClient(Integer.parseInt(message.message.split(":")[0]), message.message.split(":")[1]);
                    }
                    else if (command.equals("ginfo"))
                    {
                        int groupID = Integer.parseInt(message.getArg(1));

                        if (!groupChatRooms.containsKey(groupID))
                        {
                            addGroupChatRoom(groupID, message.getArg(2));
                        }
                        //send message
                    }
                    else if (command.equals("gmessage"))
                    {
                        ChatRoom chatRoom = groupChatRooms.get(Integer.parseInt(message.getArg(2)));
                        System.out.println(chatRoom);
                        chatRoom.groupRecieveMessage(message.message, Integer.parseInt(message.getArg(1)));
                    }
                    else if (command.equals("closeclient"))
                    {
                        int leavingClient = Integer.parseInt(message.message);
                        This.gui.knownClients.remove(leavingClient);
                        This.gui.updateList();
                        if (privateChatRooms.containsKey(leavingClient))
                        {
                            privateChatRooms.get(leavingClient).closeChatRoom();
                            privateChatRooms.remove(leavingClient);
                        }
                    }
                    else if (command.equals("file"))
                    {
                        try
                        {
                            String filePath = message.message.split(":")[0];
                            int fileSize = Integer.parseInt(message.message.split(":")[1]);

                            new FileTransferProtocol().getFile(new File(filePath), fileSize, in);
                        } catch (Exception e)
                        {
                            //e.printStackTrace();
                        }
                    }
                }
            }
        });
    }

    public void addChatRoom(ChatRoom chatRoom)
    {
        privateChatRooms.put(chatRoom.otherID, chatRoom);
    }

    public void addGroupChatRoom(int groupID, String groupName)
    {
        ChatRoom groupChatRoom = new ChatRoom(This, groupID, true);
        groupChatRoom.setVisible(true);
        groupChatRoom.setTitle(groupID + ": " + groupName);
        gui.knownGroups.put(groupID, groupName);
        gui.updateList();

        groupChatRooms.put(groupChatRoom.otherID, groupChatRoom);

    }

    /**
     * Makes the clientSocket start listening for incoming messages
     */
    public void startListening()
    {
        /*
		 * This is not in the constructor because starting a thread in the constructor is
		 * possibly giving out a reference to your object before it is fully constructed.
         */
        serverListener.start();
    }

    public void sendToServer(String header, String message)
    {
        try
        {
            out.writeChars(header + "\n" + message + "\n");
        } catch (IOException ex)
        {
            ex.printStackTrace();
        }
    }

    public void sendFileToServer(File file, int receiverID)
    {
        sendToServer("file:" + this.id + ":" + receiverID, file.getName() + ":" + file.length());
        try
        {
            new FileTransferProtocol().sendFile(file, out);
        } catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    private Message receiveFromServer()
    {
        String line;

        try
        {
            line = brinp.readLine() + "\n";
            line += brinp.readLine();

            return new Message(line);
        } catch (IOException e)
        {
            e.printStackTrace();
            return new Message("q\n\n");
        }
    }

    public void closeClient()
    {
        try
        {
            sendToServer("closeclient::", id + "");
            socket.close();
            out.close();
            in.close();
        } catch (Exception ex)
        {
        }
    }

}
