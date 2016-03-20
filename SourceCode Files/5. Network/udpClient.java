import java.io.*;	
import java.net.*;
import java.util.ArrayList;
import java.util.List;
class UdpClient implements Runnable
{  
	
		private int portNumber1;
		private int choice1;
		private UdpServer serv;

	String serverHostname = new String ("127.0.0.1");
	
	
	public UdpClient(int portNumber1, int choice1, UdpServer ser) {
		this.portNumber1=portNumber1;
		this.choice1=choice1;
		this.serv=ser;
		
	}

	public void udpclient(int portNumber1,int choice1,UdpServer thread)throws IOException{
		DatagramSocket Sock= new DatagramSocket(); 
		switch (choice1) {
		case 1:	
		
		List<Double> avg =new ArrayList<Double>();
		 List<Double> avgthr1= new ArrayList<Double>();
		send1Byte(portNumber1, Sock,avg, avgthr1);
		double avgTime=calculateAvgTime(avg);
		double avgThrb=calculateAvgTime(avgthr1);
		System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
		System.out.println("Average Time Taken = "+ avgTime);
		System.out.println("Average Throughput = "+ avgThrb);
		thread.getSock().close();
		break;
	case 2:	
		List<Double> average =new ArrayList<Double>();
		 List<Double> avgthr2= new ArrayList<Double>();
		send1KB(portNumber1, Sock,average,avgthr2);
		double avgTime1=calculateAvgTime(average);
		double avgThrb1=calculateAvgTime(avgthr2);
		System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
		System.out.println("Average Time Taken = "+ avgTime1);
		System.out.println("Average Throughput = "+ avgThrb1);
		thread.getSock().close();
		break;
	
	case 3:	
		List<Double> averages =new ArrayList<Double>();
		 List<Double> avgthr3= new ArrayList<Double>();
		send64KB(portNumber1, Sock,averages,avgthr3);
		double avgTime2=calculateAvgTime(averages);
		double avgThrb2=calculateAvgTime(avgthr3);
		System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
		System.out.println("Average Time Taken = "+ avgTime2);
		System.out.println("Average Throughput = "+ avgThrb2);
		thread.getSock().close();
		break;
	}
	
} 

	private void send1KB(int portNumber1, DatagramSocket Sock,List<Double> average,List<Double> avgthr2)
			throws SocketException, UnknownHostException, IOException {
		double blockSize=1;
		serv.start();
		
		byte[] buffer1 = new byte[1024];
		for(int e=0;e<1024;e++){
			buffer1[e]= (byte)(1);		
		};
		for (int j = 0; j < 10;j++) {
		long t1 = System.nanoTime();
		   DatagramPacket sendPacket1 = 
			   new DatagramPacket(buffer1, buffer1.length, InetAddress.getByName(serverHostname),portNumber1); 
		   Sock.send(sendPacket1); 
		   long t2 = System.nanoTime();
				long t3 = t2 - t1;
				double t4 = t3 * Math.pow(10, -9);
				average.add(new Double(t4));
				double throughput = ((blockSize*8) / (t4 * 1024 * 1024));
				avgthr2.add(new Double (throughput));
		}
	}
	private void send64KB(int portNumber1, DatagramSocket Sock,List<Double> averages,List<Double> avgthr3
			) throws SocketException, UnknownHostException,
			IOException {
				double blockSize3=61500;
				serv.start();
		
		byte[] buffer2 = new byte[61500];
		for(int e=0;e<61500;e++){
			buffer2[e]= (byte)(1);
		};
		for (int j = 0; j < 10;j++) {
		long t1 = System.nanoTime();
	System.out.println("\nStart time :  " + t1);
		   DatagramPacket sendPacket2 = 
			   new DatagramPacket(buffer2, buffer2.length, InetAddress.getByName(serverHostname),portNumber1); 
		   Sock.send(sendPacket2); 
		   long t2 = System.nanoTime();
			long t3 = t2 - t1;
			double t4 = t3 * Math.pow(10, -9);
			averages.add(new Double(t4));
			double throughput = ((blockSize3*8) / (t4 * 1024 * 1024));
			avgthr3.add(new Double (throughput));
		}
	}
	private void send1Byte(int portNumber1, DatagramSocket Sock,
			 List<Double> avg, List<Double> avgthr1) throws SocketException, UnknownHostException,
			IOException {
		double blockSize=1;
		serv.start();
		byte[] buffer = new byte[] {1};
		
		for (int j = 0; j < 10; j++)
		{
		long t1 = System.nanoTime();
		   DatagramPacket sendPacket = 
			   new DatagramPacket(buffer, buffer.length, InetAddress.getByName(serverHostname),portNumber1); 
		   Sock.send(sendPacket);
		   long t2 = System.nanoTime();
			long t3 = t2 - t1;
			double t4 = t3 * Math.pow(10, -9);
			avg.add(new Double(t4));
			double throughput = ((blockSize*8) / (t4 * 1024 * 1024));
			avgthr1.add(new Double (throughput));
	}
	}
	private static double calculateAvgTime(List<Double> avg)
	{
		double sum=0;
		for (Double value : avg) {
			sum= sum+value.doubleValue();
		}
		return sum/avg.size();	
	}

	public void run() {
		try {
			udpclient(this.portNumber1,this. choice1, this.serv);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
	
}

