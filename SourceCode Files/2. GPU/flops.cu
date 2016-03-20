//
// flops.cu
// GPU Benchmarking
//
// Created by Ronakkumar Makadiya, Kaustubh Bojewar, Sourabh //CHougale 
// Created on 09/09/2104
// Copyright (c) 2014 Ronakkumar Makadiya. All rights reserved.
//

#include <sys/time.h>
#include <iostream>
#include <cuda.h>
#include <ctime>
using namespace std;
#define SIZE 10000000


__global__ void emptyLoopTime(int n)
{

	long int i=0;
	int a=0;
	for(i=0;i<n;i++)
	{
	}

}

__global__ void flopsCUDA(long int* total,int n)
{
	//clock_t t1,t2,total_time=0;
	long int i=0;
	int a=0;
	
	for(i=0;i<n;i++)
	{
		a=a+0.5;
	}

}


void calculateFlops() 
{

	long int total=0;
	long int *d_total;
	double time=0;
	double *d_time;

	//cudaError_t cudaStatus;
	cudaMalloc(&d_total, sizeof(long int));
	cudaMalloc(&d_time, sizeof(double));

	cudaMemcpy(d_total,&total, sizeof(long int),cudaMemcpyHostToDevice);
	cudaMemcpy(d_time,&time, sizeof(double),cudaMemcpyHostToDevice);
//------------------------------------------------------------------------
	cudaEvent_t empty_start, empty_stop;
	cudaEventCreate(&empty_start);
	cudaEventCreate(&empty_stop);

// Start record
	
	cudaEventRecord(empty_start, 0);
	
	emptyLoopTime<<<1,1>>>(SIZE);

	cudaEventRecord(empty_stop, 0);

	cudaEventSynchronize(empty_start); //optional
	cudaEventSynchronize(empty_stop);

	float emptyloop;
	cudaEventElapsedTime(&emptyloop,empty_start,empty_stop);

	cudaEventDestroy(empty_start);
	cudaEventDestroy(empty_stop); 
	//cout << "Empty Loop time:"<<emptyloop<<endl;

//------------------------------------------------------------------
	cudaEvent_t start,stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
//	cudaStatus = cudaDeviceSynchronize();
   	 

	cudaEventRecord(start, 0);
	flopsCUDA<<<1,1>>>(d_total,SIZE);
	cudaEventRecord(stop, 0);

	cudaEventSynchronize(start); //optional
	cudaEventSynchronize(stop);

	
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop); // that's our time! Clean up:
	
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

//-----------------------------------------------------------------------
	//cout << "Time elapsed:"<<elapsedTime<<endl;

	cudaMemcpy(&total,d_total,sizeof(long int),cudaMemcpyDeviceToHost);
	cudaMemcpy(&time,d_time,sizeof(double),cudaMemcpyDeviceToHost);

	long double flops=SIZE/(emptyloop-elapsedTime);

	cout<<"\nFLOPS:"<<flops<<endl;

	double gflops=flops/1000000000;

	cout<< "\n The GFLOPS:"<<gflops;
	//cout<<"\nThe answer is "<<total<<endl;
//cout<<"The answer is "<<b<<endl;

	cudaFree(d_total);
	cudaFree(d_time);
	

}


int main(){

	cout <<"\n\n---------------------GFLOPS CUDA Benchmarking------------------------------------\n\n";
	calculateFlops();


	//cout <<"\n\n---------------------GIOPS CUDA Benchmarking------------------------------------\n\n";	
	//calculateIops();

	
	
	return 0;
}

