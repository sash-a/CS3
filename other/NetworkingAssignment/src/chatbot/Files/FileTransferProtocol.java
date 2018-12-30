package chatbot.Files;

import java.io.*;

public class FileTransferProtocol
{
	/**
	 * Send file and write it to the disk.
	 *
	 * @param file     that will be written to disk (i.e location).
	 * @param fileSize size of the incoming file.
	 * @param in       stream in which the bytes are received.
	 * @return false if there was a problem when try to create the file in the given location, true otherwise.
	 * @throws IOException
	 */
	public boolean getFile(File file, int fileSize, InputStream in) throws IOException
	{
		FileOutputStream fos = new FileOutputStream(file);

		byte[] buffer = new byte[fileSize];

		/*
		 * It the file does not exist try to create the file
		 */
		System.out.println(file.exists());
		if (!file.exists() & !file.createNewFile())
		{
			return false;
		}

		in.read(buffer, 0, fileSize);
		fos.write(buffer, 0, fileSize);

		return true;
	}

	/**
	 * Sends a file through an output stream as a byte array.
	 *
	 * @param file to be .
	 * @param out  stream in which the byte array will be sent.
	 * @throws IOException
	 */
	public void sendFile(File file, OutputStream out) throws IOException
	{
		FileInputStream fis = new FileInputStream(file);
		BufferedInputStream bis = new BufferedInputStream(fis);

		int count;
		byte[] buffer = new byte[(int) file.length()];

		while((count = bis.read(buffer)) > 0)
		{
			out.write(buffer, 0, count);
		}
		out.flush();
		fis.close();
		System.out.println("sent");
	}

}
