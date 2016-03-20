import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class menu {
	
	public static void main(String args[]) throws IOException, InterruptedException {
		BufferedReader a;
		a = new BufferedReader(new InputStreamReader(System.in));
		while(true){
			System.out.println("\nSelect transmission protocol");
			System.out.println("\n1. TCP");
			System.out.println("\n2. UDP");
			System.out.println("\n3. Exit");
			String s1 = a.readLine(); // taking users preference for transmission protocol.
			int ch = Integer.parseInt(s1);
			switch (ch) {
			case 1:
				
				System.out.println("Please Enter Number of Thread");
				int threadCount=Integer.parseInt(a.readLine());// Taking number of threads.
				int portNumberArray[]=new int[threadCount]; // start and initialize port number array to take port number for each port.
				for(int index=0;index<threadCount;index++){
					System.out.println("Please Enter portNumber for running Server "+(index+1));
					portNumberArray[index]=Integer.parseInt(a.readLine());// Taking port number from user.
				}
				int choice =selectBlockSize(a);	// Take a block size to be transfer.
				for (int i = 0; i < portNumberArray.length; i++) {
				tcpServer ser=new tcpServer(portNumberArray[i]);
				ser.setName("Server"+(i+1));
				ser.start();
				Thread client =new Thread(new tcpClient(portNumberArray[i],choice,ser));;
				client.start();// Starting thread
				client.join();// joining multiple threads in case of multithreading to achieve synchronization.
			}
	
				break;
				case 2:
				System.out.println("Please Enter Number of Thread");
				int threadCount1=Integer.parseInt(a.readLine());// Taking number of threads.
				int portNumberArray1[]=new int[threadCount1];// start and initialize port number array to take port number for each port.
				for(int index=0;index<threadCount1;index++){
					System.out.println("Please Enter portNumber for running Server "+(index+1));
					portNumberArray1[index]=Integer.parseInt(a.readLine()); // Taking port number from user.
				}
				int choice1 =selectBlockSize(a);   // Take a block size to be transfer.
				int blockSize=choice1== 1 ? 1:(choice1==2?1024:65530);
				for (int i = 0; i < portNumberArray1.length; i++) {
				UdpServer ser=new UdpServer(portNumberArray1[i],blockSize);
				ser.setName("Server"+(i+1));
				Thread client =new Thread(new UdpClient(portNumberArray1[i],choice1,ser));;
				client.start();// Starting thread
				client.join();// joining multiple threads in case of Multithreading to achieve synchronization.
			}			
				break;
			case 3:
				System.out.println("Exiting The Application");
				System.exit(0);
				break;
			default:
				System.out.println("Please Enter valid input");
				break;
			}
		}
		}

	private static int selectBlockSize(BufferedReader a) throws NumberFormatException, IOException {
		System.out.println("Please Select the Operation");
		System.out.println("\nSelect Block Size to Send to the Servers");
		System.out.println("\n1. 1byte");
		System.out.println("\n2. 1kilobyte");
		System.out.println("\n3. 64Kilobytes");	
		int choice =Integer.parseInt(a.readLine());
		return choice;
	}

	}
