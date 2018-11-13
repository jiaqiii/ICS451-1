import java.io.*;
import java.net.*;
public class client {
    public static void main(String[] args) throws IOException {
        // creates a socket for the client to connect
        Socket client_socket;
        // Creates a buffered reader to take in input
        BufferedReader in;
        // first arg is parsed as port number
        int port = Integer.parseInt(args[0]);
        // connects to localhost socket
        client_socket = new Socket("localhost", port);
        // takes in input to buffer reader, then prints it to the screen
        in = new BufferedReader(new InputStreamReader(client_socket.getInputStream()));
        StringBuffer string_buffer = new StringBuffer();
        String line;
        while ((line = in.readLine()) != null) {
            string_buffer.append(line);
            string_buffer.append("\n");
        }
        System.out.println(string_buffer.toString());
        in.close();
        // closing client
        client_socket.close();
    }
}
