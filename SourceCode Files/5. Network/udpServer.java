

import java.io.IOException;
import java.net.*;
class UdpServer extends Thread implements Runnable
{
	private int portNumber1;
	DatagramSocket Sock;
	DatagramPacket Dp;

	byte[] receiveData ; 
 
public void startUDPServer(int portNumber1) throws IOException
{
 
	
 

         DatagramPacket receivePacket = 
            new DatagramPacket(receiveData, receiveData.length);
         
         Sock.receive(receivePacket); 

         byte[] sentence = (byte[])(receivePacket.getData()); 
 
         InetAddress IPAddress = receivePacket.getAddress(); 
 
         int port = receivePacket.getPort(); 
 
         System.out.println ("From: " + IPAddress + ":" + port);
         System.out.println ("Received" + sentence.length +"bytes");
         //Sock.close();

}

public UdpServer(int portNumber1,int size) throws SocketException {	
	
	this.portNumber1=portNumber1;
	Sock = new DatagramSocket(portNumber1);
	receiveData=new byte[size];
}

public void run() {
	try {
		startUDPServer(this.portNumber1);
	} catch (IOException e) {
		e.printStackTrace();
	}
	
}

public DatagramSocket getSock() {
	return Sock;
}

public void setSock(DatagramSocket sock) {
	Sock = sock;
}


}