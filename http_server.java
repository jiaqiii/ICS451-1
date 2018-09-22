import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
public class http_server{ 
    public static void main(String[] args) throws Exception { 
        // listens on port 8080
        final ServerSocket server = new ServerSocket(8080);
        System.out.println("Listening for connection on port 8080 ...."); 
        while (true){ // spin forever 
            Socket client_socket = server.accept();
            InputStreamReader isr = new InputStreamReader(client_socket.getInputStream());
            BufferedReader reader = new BufferedReader(isr);
            String line = reader.readLine();
            while(!line.isEmpty()) {
                System.out.println(line);
                line = reader.readLine();
            }
            /*
            1. Read HTTP request from the client socket
            2. Prepare an HTTP response
            3. Send HTTP response to the client
            4. Close the socket
            */
        } 
    } 
}

