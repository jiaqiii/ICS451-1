import java.io.*;
import java.util.Scanner;
import java.net.ServerSocket;
import java.net.Socket;
public class http_server{ 
    public static void main(String[] args) throws Exception { 
        int port = Integer.parseInt(args[0]);
        String sending_msg = "Receiving message from server\n\n";
        String connection_closed = "Connection Closed by server.";
        System.out.println("Listening for connection on port "+port+"..."); 
        // Reading msg file into scanner to be put into a String
        Scanner scanner = new Scanner(new File("passwords.txt"), "UTF-8");
        String msg = scanner.useDelimiter("\\A").next();
        scanner.close();
        // open up port for listening
        ServerSocket server = new ServerSocket(port);
        // Port is kept open 
        while(true) {
            try (Socket socket = server.accept()) {
                // Sending message sent message to connected client
                socket.getOutputStream().write(sending_msg.getBytes("UTF-8"));
                // Sending file to connected client
                socket.getOutputStream().write(msg.getBytes("UTF-8"));
                // Sending closing connection message to disconnecting client
                socket.getOutputStream().write(connection_closed.getBytes("UTF-8"));
            }
        }
    }
}

