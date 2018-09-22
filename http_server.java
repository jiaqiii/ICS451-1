import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class http_server{ 
    public static void main(String[] args) throws Exception { 
        // listens on port 8080
        //final ServerSocket server = new ServerSocket(8080);
        System.out.println("Listening for connection on port 8080 ...."); 
        ServerSocket server = new ServerSocket(8080);
        String text = "text.txt";
        String line = null;
            //Socket client_socket = server.accept();
            try {
                FileReader fileReader = new FileReader(text);
                BufferedReader reader = new BufferedReader(fileReader);
                line = reader.readLine();
                while((line != null)) {
                    try (Socket socket = server.accept()) {
                        socket.getOutputStream().write(line.getBytes("UTF-8"));
                        line = reader.readLine();
                    }
                }
                if(line == null) {
                    System.out.println("Connection Closed\n");
                }
                reader.close();
            }

            catch(FileNotFoundException e) {
                System.out.println("File Not Found");
            }
/*
            InputStreamReader isr = new InputStreamReader(client_socket.getInputStream());
            BufferedReader reader = new BufferedReader(isr);
            line = reader.readLine();
            while(!line.isEmpty()) {
                socket.getOutputStream().write(line);
                line = reader.readLine();
            }
*/
            /*
            1. Read HTTP request from the client socket
            2. Prepare an HTTP response
            3. Send HTTP response to the client
            4. Close the socket
            */
        } 
}

