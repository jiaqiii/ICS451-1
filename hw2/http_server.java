import java.io.*;
import java.util.Scanner;
import java.net.ServerSocket;
import java.net.Socket;
public class http_server{ 
    public static void main(String[] args) throws Exception { 
        // Parse port argument as int
        int port = Integer.parseInt(args[0]);
<<<<<<< refs/remotes/origin/hw4:hw2/http_server.java
        String sending_msg = "Receiving message from server\n\n";
        String connection_closed = "Connection Closed by server.";
        System.out.println("Listening for connection on port "+port+"..."); 
        // Reading msg file into scanner to be put into a String
=======

        // Lets client know message is coming and when it is fully delivered
        // Will be printed onto client screen
        String sending_msg = "Receiving message from server.\n\n";
        String sent_msg = "Message was fully received above.\n";

        // open up port for listening
        ServerSocket server = new ServerSocket(port);
        System.out.println("Listening for connection on port "+port+"..."); 

        // Reading text file into scanner to be put into a String
>>>>>>> Refined server messages and comments:http_server.java
        Scanner scanner = new Scanner(new File("ArduinoIDE_guide_galileo.html"), "UTF-8");
        String msg = scanner.useDelimiter("\\A").next();
        scanner.close();

        // Port is kept open for connection
        while(true) {
<<<<<<< refs/remotes/origin/hw4:hw2/http_server.java
            try (Socket socket = server.accept()) {
                // Sending file to connected client
                socket.getOutputStream().write(sending_msg.getBytes("UTF-8"));
                socket.getOutputStream().write(msg.getBytes("UTF-8"));
                socket.getOutputStream().write(connection_closed.getBytes("UTF-8"));
            }
=======
            // Created socket to accept connections from client
            Socket socket = server.accept(); 

            // Sending file to connected client
            socket.getOutputStream().write(sending_msg.getBytes("UTF-8"));
            socket.getOutputStream().write(text.getBytes("UTF-8"));
            socket.getOutputStream().write(sent_msg.getBytes("UTF-8"));

            // Close socket and connection
            socket.close();
>>>>>>> Refined server messages and comments:http_server.java
        }
    }
}

