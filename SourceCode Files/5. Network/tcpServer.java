import java.net.*;
import java.io.*;
//import java.util.*;


public class tcpServer extends Thread implements Runnable {
	private int portNumber;
	public ServerSocket server; 
	
	public void startServer(int portNumber) throws IOException {
		server=new ServerSocket(portNumber);
		
		Socket client = server.accept();
		DataOutputStream e=new DataOutputStream(client.getOutputStream());
		DataInputStream b = new DataInputStream(client.getInputStream());
		
		while(true){		
				b.read();
				String st2="Data received successfully";
				e.writeUTF(st2);
				e.flush();
		}

	}
	public void run() {
		try {
			startServer(this.portNumber);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	public tcpServer(int portNumber) {	
		this.portNumber=portNumber;
	}
	public ServerSocket getServer() {
		return server;
	}
	public void setServer(ServerSocket server) {
		this.server = server;
	}
}