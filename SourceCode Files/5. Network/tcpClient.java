import java.net.*;
import java.util.ArrayList;
import java.util.List;
import java.io.*;
//import java.util.*;
public class tcpClient implements Runnable 
{	
        private static int count=-1;
		private int portNumber;
		private int choice;
		private tcpServer thread;

		public void client(int portNumber,int choice,tcpServer thread ) throws IOException{
                       
			Socket client=new Socket("localhost",portNumber);
			DataInputStream b = new DataInputStream(client.getInputStream());
			double blockSize;
       
				switch (choice) {
				case 1:
					 blockSize = 1;
					List<Double> avg =new ArrayList<Double>();
					 List<Double> avgthr1= new ArrayList<Double>();
					
					avgthr1=sendAndReadByte(client, b, blockSize, avg, avgthr1);
					double avgTime=calculateAvgTime(avg);
					double avgThrb=calculateAvgTime(avgthr1);
					System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
					System.out.println(" Average Time Taken = "+ avgTime);
					System.out.println("Average Throughput = "+ avgThrb);		
					thread.getServer().close();
					break;
					
				case 2:
					 blockSize=1024;
					List<Double> average= new ArrayList<Double>();
					 List<Double> avgthr2= new ArrayList<Double>();
					sendReceivekb(client, b, blockSize, average, avgthr2);
					double avgTimekb=calculateAvgTime(average);
					double avgThrkb=calculateAvgTime(avgthr2);
					System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
					System.out.println("Average Time Taken = "+ avgTimekb);
					System.out.println("Average Throughput = "+ avgThrkb);
					thread.getServer().close();
					break;

				case 3:
					    blockSize=65536;
					    List<Double> averages= new ArrayList<Double>();
					    List<Double> avgthr3= new ArrayList<Double>();
					    sendRead64kb(client, b, blockSize, averages, avgthr3);
					    double avgTime64kb=calculateAvgTime(averages);
					    double avgThr64kb=calculateAvgTime(avgthr3);
					    System.out.println("\n*********BenchMarking Result for "+thread.getName()+"*************");
					    System.out.println("Average Time Taken = "+ avgTime64kb);
					    System.out.println("Average Throughput = "+ avgThr64kb);
					break;
				default:

					break;
				}			
		}	              
              



	private static void sendRead64kb(Socket client, DataInputStream b,
			double blockSize, List<Double> averages,List<Double> avgthr3) throws IOException {
		++count;
		byte [] z1 = new byte[65536]; 
			for(long d=0;d<65536;d++)
			{
				z1[(int) d]= (byte) (1);
			};
			DataOutputStream p = new DataOutputStream(client.getOutputStream());
		
		long t1 = System.nanoTime();
		
		p.write(z1);
		p.flush();
		String s2 = b.readUTF();
		System.out.println(":  " + s2);
		long t2 = System.nanoTime();
		
		long t3 = t2 - t1;
		
		double t4 = t3 * Math.pow(10, -9);
		averages.add(new Double(t4));
		
		double throughput = ((blockSize*8) / (t4 * 1024 * 1024));
		avgthr3.add(new Double (throughput));
		
		if(count<10){
		sendReceivekb(client, b, blockSize, averages, avgthr3);
		}
		
	}

		

		private static void sendReceivekb(Socket client, DataInputStream b,
				double blockSize, List<Double> average, List<Double> avgthr2) throws IOException {
			byte [] kb = new byte[1024];
			for(int e=0;e<1024;e++){
				kb[e]= (byte)(1);
			};
			DataOutputStream p = new DataOutputStream(client.getOutputStream());
			for (int j = 0; j < 10; j++) {		
				long t1 = System.nanoTime();
				p.write(kb);
				long t2 = System.nanoTime();
				long t3 = t2 - t1;
				double t4 = t3 * Math.pow(10, -9);
				average.add(new Double(t4));
			
				double throughput = ((blockSize*8) / (t4 * 1024 * 1024));
				avgthr2.add(new Double (throughput));
			
			}
		}

		private static double calculateAvgTime(List<Double> avg) {
			double sum=0;
			for (Double value : avg) {
				sum= sum+value.doubleValue();
			}
			return sum/avg.size();
			
		}

		private static List<Double> sendAndReadByte(Socket client, DataInputStream b,
 double blockSize, List<Double> avg,
 List<Double> avgthr1) throws IOException {
			byte[] z = new byte[] { 1 };
			DataOutputStream p = new DataOutputStream(client.getOutputStream());
			for (int j = 0; j < 10; j++) {
				long t1 = System.nanoTime();
				p.write(z);
				p.flush();
				b.readUTF();

				long t2 = System.nanoTime();

				long t3 = t2 - t1;

				double t4 = t3 * Math.pow(10, -9);
				avg.add(new Double(t4));

				double throughput = ((blockSize*8) / (t4 * 1024 * 1024));
				avgthr1.add(new Double(throughput));

			}
			return (avgthr1);

		}



		public void run() {
		try{
			client(this.portNumber , this.choice ,this.thread);
		} catch (IOException e){
			e.printStackTrace();
		}
			
		}
		public tcpClient(int portNumber,int choice,tcpServer thread){
			this.portNumber=portNumber;
			this.choice = choice;
			this.thread=thread;
		}
	
}


