// Memorycuda.cu
// GPU Benchmarking
//
// Created by Ronakkumar Makadiya, Kaustubh Bojewar, Sourabh //CHougale 
// Created on 09/09/2104
// Copyright (c) 2014 Ronakkumar Makadiya. All rights reserved.
//


#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void calclulate(float timeTaken, int no_of_threads){
	float data = 0.001;
	printf("\n%f GB/sec", (data / (timeTaken / 1024.0))*no_of_threads);

}


__global__ void readwritebyte(char *str, int *size){

	char* a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char str2 = a_to_z[0];
	char *s = &str2;
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	
	for (int i = 0; i < 1024*1024; i++){
		memcpy(&str[index], s, sizeof(char));
	}
	
	//free(s);
}



void startkernal(int threads,int blocks,int* size)
{

	//cudaError_t cudaStatus;
	cudaEvent_t start, stop;
	char *str_d;
	int *size_d;
	float time;

	cudaMalloc((void**)&str_d, *size * sizeof(char));
	cudaMalloc((void**)&size_d, sizeof(int));
	cudaMemcpy(size_d,size, sizeof(int), cudaMemcpyHostToDevice);


		
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	readwritebyte <<<blocks, threads >>>(str_d,size_d);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);
	printf("\nTime for read and write one Byte kernel: %f ms", time);

	calclulate(time, blocks*threads);
}	


int main()
{

	int num_of_blocks=1024;
	int num_of_threads_block=1024;
	int mem_size=1024*1024*1024;

	startkernal(num_of_threads_block,num_of_blocks,&mem_size);

	return 0;

}
