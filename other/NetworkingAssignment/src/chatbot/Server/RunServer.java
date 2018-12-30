package chatbot.Server;

import java.io.File;
import java.io.IOException;
import java.net.Socket;

/**
 * @author abrsas002
 */
public class RunServer
{

	public static void main(String[] args)
	{
		Server server;

		Socket newClientSocket = null;
		server = new Server();

		// Making the file storage directory
		File storageDir = new File("file_storage");
		if (!storageDir.exists() & !storageDir.mkdir())
		{
			// If server cannot create storage directory then kill it
			return;
		}

		while(true)
		{
			try
			{
				newClientSocket = server.serverSocket.accept(); //listening for new client connection
			}
			catch (IOException e)
			{
			}
			// new thread for a new clientSocket
			System.out.println("Adding client ");
			server.addClient(new ClientListener(newClientSocket, server));
			System.out.println(server.clients.size());
		}
	}
}
