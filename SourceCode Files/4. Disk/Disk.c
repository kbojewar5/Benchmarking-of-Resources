#include<stdio.h>
#include<math.h>
#include<limits.h>
#include<time.h>
#include<pthread.h>
#include<string.h>
//#include<fstream.h>

#define BYTE 1
#define KB 1024
#define MB 1048576

#define MAX_BYTE 10000000
#define MAX_KB 1000
#define MAX_MB 10

#define MAX_BYTE_WR 100000
#define MAX_KB_WR 1000
#define MAX_MB_WR 10

double throughput,latency,latency1,throughput1;  

pthread_mutex_t lock;
int no_of_threads;

struct disk_str
{
int thread_id;
long int type;
long int max;
long int max_wr;
};


void *operate_random(void *arguments1)
{
	int ch1,j;
	pthread_t threads[no_of_threads];
	int result;
	int thread_args[no_of_threads];
	struct disk_str *args = (struct disk_str *)arguments1;
	FILE *fp,*fp1;
	//Random Read and Write 1Byte Data
  
	char buffer[1024*1024];			
	clock_t t1,t2,total_time=0,total_time1=0;
	
	double throughput=0.0;
	int k,l;
	fp= fopen("test.txt", "r");
	//fseek(fp,r,SEEK_SET);
	//long temp=0;
	int s,r;
     	for(k=0;k<args->max;k++)
	{
		s=rand()%3000;
		t1=clock();		
		fseek(fp,s,SEEK_SET);
		fread(buffer,1,args->type,fp);
		t1=clock()-t1;
		total_time+=t1; 
		
        }
	fclose(fp);
	double intloop =((double)total_time)/CLOCKS_PER_SEC;
	//printf("%f\n",intloop);
	latency=latency+intloop;

	fp=fopen("test.txt","r");
	fp1=fopen("rand_test.txt","r+");
	fseek(fp,0,SEEK_SET);	
	for(l=0;l<args->max_wr;l++)
	{
		r= rand()%3000;
		fread(buffer,1,args->type,fp);
		t1=clock();
		fseek(fp1,r,SEEK_SET);
		fwrite(buffer,1,args->type,fp1);
		t1=clock()-t1;
		total_time1+=t1;
	}
	fclose(fp1);
	fclose(fp);
	double intloop1=((double)total_time1)/CLOCKS_PER_SEC;
	latency1=latency1+intloop1;	
}

							
void *operations(void *arguments)
{
        //volatile int i=0;
	int ch1,j;
	pthread_t threads[no_of_threads];
	int result;
	int thread_args[no_of_threads];
	struct disk_str *args = (struct disk_str *)arguments;
	FILE *fp,*fp1;
	//Sequential Read and Write 1Byte Data
  
	char buffer[1024*1024];			
	clock_t t1,t2,total_time=0,total_time1=0;;
	double throughput=0.0;
	int k,l;
	fp= fopen("test.txt", "r");
	fseek(fp,0,SEEK_SET);
	
 	for(k=0;k<args->max;k++)
	{
	
		t1= clock();
		fread(buffer,1,args->type,fp);
		t1=clock()-t1;
		total_time+=t1;
        }
	fclose(fp);
	double intloop =((double)total_time)/CLOCKS_PER_SEC;
	//printf("%f\n",intloop);
	latency=latency+intloop;

	
	fp=fopen("test.txt","r");
	fp1=fopen("test1.txt","w+");
	fseek(fp,0,SEEK_SET);
	for(l=0;l<args->max_wr;l++)
	{
		
		fread(buffer,1,args->type,fp);
		t1=clock();
		fwrite(buffer,1,args->type,fp1);
		t1=clock()-t1;
		total_time1+=t1;	
	}
	fclose(fp1);
	
	double intloop1=((double)total_time1)/CLOCKS_PER_SEC;
	latency1=latency1+intloop1;	
}


int main()
{
struct disk_str args;
int ch,ch1,ch2,i,result;
//int thread_args[no_of_threads];
pthread_t threads[no_of_threads];
pthread_mutex_init(&lock, NULL);
	
		printf("..Menu...\n");
		printf("1.SEQUENTIAL\n");
		printf("2.RANDOM\n");
		printf("3.EXIT\n");
		printf("\nEnter your choice :");
		scanf("%d",&ch);


		switch(ch)
		{

			case 1:printf("You have selected SEQUENTIAL ACCESS\n");
			       printf("\nEnter the number of threads (1,2,4)");
			       scanf("%d",&no_of_threads);
			       pthread_setconcurrency(no_of_threads);
			       printf("\n..MENU..");
			       printf("\n1. 1 BYTE");
		               printf("\n2. 1 KB");
		               printf("\n3. 1 MB");
		               printf("\n Enter your choice");
		               scanf("%d",&ch1);
			       switch(ch1)
			       {
					case 1: printf("\nSequential Read and Write Operations for 1BYTE");			
			       		        for(i = 1; i <= no_of_threads; i++)
						{	
       	       				  
							args.thread_id = i;
							args.type=BYTE;
							args.max= MAX_BYTE;
							args.max_wr=MAX_BYTE_WR;
							result = pthread_create(&threads[i], NULL,operations,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						double f_latency= (latency*1000)/MAX_BYTE;
						printf("\nLatency = %f ms\n",f_latency);
						double avg = latency/no_of_threads;
						throughput=(MAX_BYTE)/(avg*1024*1024);
						printf("Throughput = %f MB/sec\n",throughput);
						printf("Write Sequential operations for 1BYTE\n");

						double f_latency1=(latency1*1000)/MAX_BYTE_WR;						
						printf("\nLatency= %f ms\n",f_latency1);
						double avg1 = latency1/no_of_threads;
						throughput1=(MAX_BYTE_WR)/(avg1*1024*1024);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;	
					case 2: printf("\nSequential Read and Write Operations for 1KB\n");
						 for(i = 1; i <= no_of_threads; i++)
						{	
       	       				  
							args.thread_id = i;
							args.type=KB;
							args.max=MAX_KB;
							args.max_wr=MAX_KB_WR;
							result = pthread_create(&threads[i], NULL,operations,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						f_latency=(latency*1000)/MAX_KB;
						printf("Latency = %f ms\n",f_latency);
						avg = latency/no_of_threads;
						throughput=(MAX_KB)/(avg*1024);
						printf("Throughput = %f MB/sec\n",throughput);

						f_latency1= (latency1*1000)/MAX_KB_WR;					
						printf("\nLatency= %f ms\n",f_latency1);
						avg1 = latency1/no_of_threads;
						throughput1=(MAX_KB_WR)/(avg1*1024);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;
					
					case 3: printf("\nSequential Read and Write Operations for 1MB\n");
						 for(i = 1; i <= no_of_threads; i++)
						{	
       	       				  
							args.thread_id = i;
							args.type=MB;
							args.max=MAX_MB;
							args.max_wr=MAX_MB_WR;
							result = pthread_create(&threads[i], NULL,operations,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						f_latency=(latency*1000)/MAX_MB;
						printf("Latency = %f ms\n",f_latency);
						avg = latency/no_of_threads;
						throughput=(MAX_MB)/(avg);
						printf("Throughput = %f MB/sec\n",throughput);

						f_latency1=(latency1*1000)/MAX_MB_WR;						
						printf("\nLatency= %f ms\n",f_latency1);
						avg1 = latency1/no_of_threads;
						throughput1=(MAX_MB_WR)/(avg1);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;
				}
			       break;
                     	
			   case 2:printf("You have selected RANDOM ACCESS \n");
			   printf("\nEnter the number of threads (1,2,4)");
			   scanf("%d",&no_of_threads);
			   pthread_setconcurrency(no_of_threads);
		
	   printf("\n..MENU..");
			   printf("\n1. 1 BYTE");
		           printf("\n2. 1 KB");
		           printf("\n3. 1 MB");
		           printf("\n Enter your choice");
		           scanf("%d",&ch2);
			   switch(ch2)
			   {
					case 1:printf("\nRandom Read and Write Operations for 1BYTE");
						       printf("\n\nRandom Read\n");			
			       		               for(i = 1; i <= no_of_threads; i++)
						       {	
       	       				  
							args.thread_id = i;
							args.type=BYTE;
							args.max=10000000;
							args.max_wr=100000;
							result = pthread_create(&threads[i], NULL,operate_random,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						double f_latency= (latency*1000)/10000000;
						printf("\nLatency = %f ms\n",f_latency);
						double avg = latency/no_of_threads;
						throughput=(10000000)/(avg*1024*1024);
						printf("Throughput = %f MB/sec\n\n",throughput);
						printf("Random Write\n");

						double f_latency1= (latency1*1000)/100000;						
						printf("\nLatency= %f ms\n",f_latency1);
						double avg1 = latency1/no_of_threads;
						throughput1=(100000)/(avg1*1024*1024);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;	
				
					case 2: printf("\nRandom Read and Write Operations for 1 KB");
						       printf("\n\nRandom Read");			
			       		               for(i = 1; i <= no_of_threads; i++)
						       {	
       	       				  
							args.thread_id = i;
							args.type=KB;
							args.max=150000;
							args.max_wr=1000000;
							result = pthread_create(&threads[i], NULL,operate_random,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						f_latency= (latency*1000)/150000;
						printf("\nLatency = %f ms\n",f_latency);
						avg = latency/no_of_threads;
						throughput=(1000)/(avg*1024);
						printf("Throughput = %f MB/sec\n\n",throughput);
						printf("Random Write\n");

						f_latency1= (latency1*1000)/1000000;
						printf("\nLatency= %f ms\n",f_latency1);
						avg1 = latency1/no_of_threads;
						throughput1=(1000)/(avg1*1024);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;	

					case 3:	printf("\nRandom Read and Write Operations for 1 MB");
						       printf("\n\nRandom Read");			
			       		               for(i = 1; i <= no_of_threads; i++)
						       {	
       	       				  
							args.thread_id = i;
							args.type=MB;
							args.max=100;
							args.max_wr=100;
							result = pthread_create(&threads[i], NULL,operate_random,(void *)&args);
					
						}
						for (i=1; i<=no_of_threads; ++i)
						{
						result = pthread_join(threads[i], NULL);
						}
						f_latency= (latency*1000)/100;
						printf("\nLatency = %f ms\n",f_latency);
						avg = latency/no_of_threads;
						throughput=(100)/(avg*1024);
						printf("Throughput = %f MB/sec\n\n",throughput);
						printf("Random Write\n");
						
						f_latency1= (latency1*1000)/100;
						printf("\nLatency= %f ms\n",f_latency1);
						avg1 = latency1/no_of_threads;
						throughput1=(100)/(avg1*1024);
						printf("Throughput = %f MB/sec\n",throughput1);
						pthread_mutex_destroy(&lock);
	  	      	    			break;				
                               //operation(no_of_threads,ch);
			       break;
		}
			case 3: //exit(); 
				
                               break;
		       
		}

	 
	

return 0;

}
