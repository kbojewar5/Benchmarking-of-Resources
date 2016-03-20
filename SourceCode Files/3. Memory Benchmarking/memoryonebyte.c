//
// main.cpp
// Memory Benchmarking - For 1 byte
//
// Created by Ronakkumar Makadiya (CWID - A20332994)
// Created on 09/09/2104
// Copyright (c) 2014 Ronakkumar Makadiya. All rights reserved.
//


#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include<string.h>

double sequentialThroughput=0,randomThroughput=0;

double sequentialLatency=0,randomLatency=0;


struct arg_struct {
	int thread_id;
	int blockSize;
};

int random_number(int limit)
{
	int result=rand()%limit;
	return result;
}		

char* initializeBlock(long int blocksize)
{

	const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	long int allocate_size=blocksize*10*1024;
	char* buffer = new char[allocate_size];
	long int i=0;
	for(i = 0; i < allocate_size; i++)
	{
		buffer[i] = a_to_z[random_number(26)];
	}

	return buffer;
}

void sequential() 
{
	char* block1=initializeBlock(1);
	char* block2=initializeBlock(1);
	char* temp = new char[10*1024];

	char* block1_temp=block1;
	char* block2_temp=block1;
	char* temp_temp=temp;


	volatile long int i,j;
	clock_t t1,t2,total_time=0;
	for(i=0;i<100;i++)
	{

		for(j=0;j<10*1024;j++) 
		{

			t1 = clock();
			memcpy(temp, block1, 1); //1 Read and 1Write operation.
			memset(block1, '-',1); // 1 WriteOperation
			memcpy(block1, block2,1); //1 Read and1 Write operation.
			memset(block2, '#',1); // 1 WriteOperation
			memcpy(block2, temp,1); // // 1 Readand 1 Write operation.
			t2 = (clock() - t1);
			total_time += t2;

			block1 += 1;
			block2 += 1;
			temp += 1;

		}

		block1 = block1_temp;
		block2 = block2_temp;
		temp = temp_temp;

	}

	double timeTaken =((double)total_time)/CLOCKS_PER_SEC;
	long bytesTransfer=(long)1 * 100 *10*1024*8;

	double throughput_seq=bytesTransfer/(timeTaken*1024*1024);
	//printf("\nseq:%f\n",throughput_seq);
	long double temp_latency=(timeTaken*1000)/(100 *10*1024*8);
	//printf("\nlat:0.8g%\n",temp_latency);
	sequentialThroughput+=throughput_seq;
	sequentialLatency+=temp_latency;
}

void randomrw()
{

	char* block1=initializeBlock(1);
	char* block2=initializeBlock(1);
	char* temp = new char[10*1024];

	char* block1_temp=block1;
	char* block2_temp=block1;
	char* temp_temp=temp;


	volatile long int i,j;
	clock_t t1,t2,total_time=0;
	for(i=0;i<100;i++)
	{

		for(j=0;j<10*1024;j++) 
		{

			t1 = clock();
			memcpy(temp, block1, 1); //1 Read and 1Write operation.
			memset(block1, '-',1); // 1 WriteOperation
			memcpy(block1, block2,1); //1 Read and1 Write operation.
			memset(block2, '#',1); // 1 WriteOperation
			memcpy(block2, temp,1); // // 1 Readand 1 Write operation.
			t2 = (clock() - t1);
			total_time += t2;

			block1 += (random_number(1024*10)-1)*1;
			block2 +=(random_number(1024*10)-1)*1;
			temp += (random_number(1024*10)-1)*1;


			block1 = block1_temp;
			block2 = block2_temp;
			temp = temp_temp;

		}


	}
	double timeTaken =((double)total_time)/CLOCKS_PER_SEC;
	long bytesTransfer=(long)1 * 100 *10*1014*8;

	double temp_latency=(timeTaken*1000)/(100 *10*1024*8);
	double throughput=bytesTransfer/(timeTaken*1024*1024);
	//printf("thru:ra:%f\n",throughput);

	randomThroughput+=throughput;
	randomLatency+=temp_latency;
}


void *calculateBenchmark(void *arguments)
{

	sequential();
	randomrw();

	return NULL;

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
	printf("\n\n Latency of Sequential read/write :%f",sequentialLatency/no_of_threads);

	printf("\n\n Throughput of Random read/write :%f",randomThroughput/no_of_threads);
	printf("\n\n Latency of Random read/write :%f",randomLatency/no_of_threads);
}
	
