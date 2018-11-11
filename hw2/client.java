import java.io.*;
import java.net.*;
public class client {
    public static void main(String[] args) throws IOException {
        Socket client_socket;
        BufferedReader in;

        int port = Integer.parseInt(args[0]);
        client_socket = new Socket("localhost", port);
        in = new BufferedReader(new InputStreamReader(client_socket.getInputStream()));
        StringBuffer string_buffer = new StringBuffer();
        String line;
        while ((line = in.readLine()) != null) {
            string_buffer.append(line);
            string_buffer.append("\n");
        }
        System.out.println(string_buffer.toString());
        in.close();
        client_socket.close();
    }
}
