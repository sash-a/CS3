package chatbot.Server;

import chatbot.Group;
import chatbot.Message;
import java.io.IOException;
import java.net.ServerSocket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class Server {

    public ServerSocket serverSocket = null;

    final static int PORT = 8080;
    public HashMap<Integer, ClientListener> clients = new HashMap<>();//maps client id to the client listener
    int nextClientID = 0;

    public List<Group> groups;

    public Server() {
        groups = new ArrayList<>();
        try {
            serverSocket = new ServerSocket(PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void addClient(ClientListener client) {
        clients.put(nextClientID, client);
        nextClientID++;
        client.start();
        // Sending clientSocket ID
    }

    public void sendToAllClients(String header, String message) {
        for (int id : clients.keySet()) {
            //ClientListener cli = clients.get(id);
            sendToClient(header, message, id);
        }
    }

    public void sendToClient(String header, String message, int clientID) {
        if (!clients.containsKey(clientID)) {
            System.out.println("no client with id: " + clientID + " is connected currently");
            return;
        }
        try {
            clients.get(clientID).out.writeChars(header + "\n" + message + "\n");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

}
