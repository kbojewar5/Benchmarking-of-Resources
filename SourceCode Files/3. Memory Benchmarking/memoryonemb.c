#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include<string.h>

double sequentialThroughput=0,randomThroughput=0;
double sequentialLatency=0,randomLatency=0;

int random_number(int limit)
{
	int result=rand()%limit;
	return result;
}

char* initializeBlock(long int blocksize)
{

	const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	long allocate_size;

	allocate_size=(blocksize)*100;

	char* buffer = new char[allocate_size];
	long int i=0;
	for(i = 0; i < allocate_size; i++)
	{
		buffer[i] = a_to_z[random_number(26)];
		
	}

	return buffer;
}

void sequential(char* block1, char* block2, char* temp, char* block1_temp, char* block2_temp, char* temp_temp) 
{

	volatile long int i,j;
	clock_t t1,t2,total_time=0;
	for(i=0;i<10;i++)
	{

		for(j=0;j<10;j++) 
		{

			t1 = clock();
			memcpy(temp, block1, 1024*1024); //1 Read and 1Write operation.
			memset(block1, '-',1024*1024); // 1 WriteOperation
			memcpy(block1, block2,1024*1024); //1 Read and1 Write operation.
			memset(block2, '#',1024*1024); // 1 WriteOperation
			memcpy(block2, temp,1024*1024); // // 1 Readand 1 Write operation.
			t2 = (clock() - t1);
			total_time += t2;

			block1 += 1024*1024;
			block2 += 1024*1024;
			temp += 1024*1024;

		}

		block1 = block1_temp;
		block2 = block2_temp;
		temp = temp_temp;

	}

	double timeTaken =((double)total_time)/CLOCKS_PER_SEC;
	long bytesTransfer=(long)1024*1024*10*10*8;
	double temp_latency = (timeTaken*10)/(1024*1024*8);
	double throughput=bytesTransfer/(timeTaken*1024*1024);
	
	sequentialThroughput+=throughput;
	sequentialLatency+=temp_latency ;

}

void randomrw()
{

	
	char* block1=initializeBlock(1024*1024);
	char* block2=initializeBlock(1024*1024);
	long temp_size=1024*1024*100;
	char* temp = new char[temp_size];

	char* block1_temp=block1;
	char* block2_temp=block1;
	char* temp_temp=temp;
	int max;
	volatile long int i,j;
	clock_t t1,t2,total_time=0;
	for(i=0;i<10;i++)
	{

		for(j=0;j<10;j++) 
		{

			t1 = clock();
			memcpy(temp, block1, 1024*1024); //1 Read and 1Write operation.
			memset(block1, '-',1024*1024); // 1 WriteOperation
			memcpy(block1, block2,1024*1024); //1 Read and1 Write operation.
			memset(block2, '#',1024*1024); // 1 WriteOperation
			memcpy(block2, temp,1024*1024); // // 1 Readand 1 Write operation.
			t2 = (clock() - t1);
			total_time += t2;

			//printf("%d\n",random_number(100));
			block1 += (random_number(100)-1)*1024*1024;
			block2 +=(random_number(100)-1)*1024*1024;
			temp += (random_number(100)-1)*1024*1024;



			block1 = block1_temp;
			block2 = block2_temp;
			temp = temp_temp;
		}
	}

	double timeTaken =((double)total_time)/CLOCKS_PER_SEC;
	long bytesTransfer=(long)(1024 *1024)*10*10*8;

	double throughput=bytesTransfer/(timeTaken*1024*1024);

	double temp_latency = (timeTaken*10)/(1024*1024*8);

	randomThroughput+=throughput;
	randomLatency += temp_latency ;
}


void *calculateBenchmark(void *arguments)
{
	char* block1=initializeBlock(1024*1024);
	char* block2=initializeBlock(1024*1024);
	long temp_size=1024*1024*100;
	char* temp = new char[temp_size];

	char* block1_temp=block1;
	char* block2_temp=block1;
	char* temp_temp=temp;


	sequential(block1, block2, temp,block1_temp,block2_temp,temp_temp);

	randomrw();

}


main()
{

	int no_of_threads=0;

	printf("\nEnter the number of threads :");
	scanf("%d",&no_of_threads);

	pthread_t threads[no_of_threads];
	
	int i=0;
	int result=0;
	for(i = 0; i < no_of_threads; i++)
	{

		result = pthread_create(&threads[i], NULL,calculateBenchmark,(void *)&no_of_threads);
		if (result)
		{
			printf("Error:unable to create thread %d",result);
			//exit(-1);
		}
	}
	for(i=0;i<no_of_threads;i++) 
	{
		pthread_join(threads[i],NULL);	
	}

	printf("\n\n Throughput of Sequential read/write :%f",sequentialThroughput/no_of_threads);
	printf("\n\n Latency of Sequential read/write :%0.11f",sequentialLatency/no_of_threads);	

	printf("\n\n Throughput of Random read/write :%f",randomThroughput/no_of_threads);
	printf("\n\n Latency of Random read/write :%0.11f",randomLatency/no_of_threads);
	pthread_exit(NULL);
}
