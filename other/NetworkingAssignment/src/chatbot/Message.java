package chatbot;

/**
 * @author actsha001
 * <p>
 * messages: name:: -> clientSocket tells server what its name is id:: -> server
 * tells clientSocket its id quit:: -> clientSocket tells server it is
 * disconnecting test:: -> /for test messages message:senderID:receiverID -> for
 * sending message client to server message:senderID -> for sending message
 * server to client gmessage:senderid:groupID -> sending message to server with
 * the intent of it being sent to a group amessage:senderid: -> sends message to
 * all clients on server file:senderID:recieverID -> file client to server
 * message=<file name>:<file size>
 *
 * ginfo:groupID:groupName -> list of all user ids sep = : sndclient:: -> server
 * sends clientSocket a list of connected clientSocket name ID pairs separated
 * by a ',' and each separated by a ':' closeclient:: -> message = clientID ;
 * leaving client informs the server/ srever informs all clients - that the
 * leaving client has left
 *
 * <p>
 *
 */
public class Message
{

    public String header;
    public String message;

    public Message(String lines)
    {
        lines = lines.replaceAll(" ", "");
        header = lines.split("\n")[0];
        message = lines.split("\n")[1];
    }

    public String getCommand()
    {
        return header.split(":")[0];
    }

    public String getArg(int argNo)
    {
        return header.split(":")[argNo];
    }

    @Override
    public String toString()
    {
        return "Message{" + "header=" + header + ", message=" + message + '}';
    }

    public static String encryptMessage(String message, int key)
    {
        //want restrict char value to between 65 and 122
        char[] m = message.toCharArray();
        char[] em = new char[m.length];

        String vals = "###";
        for (int i = 0; i < m.length; i++)
        {
            int val = m[i] + i + key + i * key;
            em[i] = (char) (val % 57 + 65);
            vals += (val / 57) + "@@";
        }
        vals = vals.substring(0, vals.length() - 2);
        //System.out.println("encrypting " + message + " to " + String.valueOf(em) + (vals));
        return String.valueOf(em) + (vals);
    }

    public static String decryptMessage(String encryptedMessage, int key)
    {
        //System.out.println("decrypting " + encryptedMessage);
        String[] vals = encryptedMessage.split("###")[1].split("@@");
        encryptedMessage = encryptedMessage.split("###")[0];

        char[] m = encryptedMessage.toCharArray();
        char[] em = new char[m.length];

        for (int i = 0; i < m.length; i++)
        {
            em[i] = (char) (57 * Integer.parseInt(vals[i]) - 65 + m[i] - i - key - i * key);
        }

        return String.valueOf(em);
    }
}
