//
// CPU.c
// CPU Benchmarking
//
// Created by Ronakkumar Makadiya (CWID - A20332994)
// Created on 09/09/2104
// Copyright (c) 2014 Ronakkumar Makadiya. All rights reserved.
//


#include <pthread.h>
#include <stdio.h>
#include <time.h>		//Including header files
#include <limits.h>

struct arg_struct {		// defining structure 
	int thread_id;
	int choice;
};


double gflops_avg=0.0;
double flops_avg=0.0;

double giops_avg=0.0;
double iops_avg=0.0;


void *calculateBenchMark(void *arguments)	// Calculating  Benchmarrking 
{
	clock_t t1,t2;

	struct arg_struct *args = (struct arg_struct *)arguments;  
	int i=0;
	double emptyloop,floatingloop=0,flops,gflops;
	double temp_float_value;
	t1=clock();
	for(i=0;i<INT_MAX;i++)				// calcuating  Empty loop timing
	{
	}
	t1 = clock() - t1;
	emptyloop =((double)t1)/CLOCKS_PER_SEC;

	/* ...........For Flops ..........................*/

	if(args -> choice == 0)
	{

		t1=clock(); 
								// calculating floatig point operation time
		for(i=0;i<INT_MAX;i++)
		{
			temp_float_value +=0.5;
		}
		t1 = clock() - t1;
		floatingloop =((double)t1)/CLOCKS_PER_SEC;
		flops=INT_MAX/((floatingloop-emptyloop));
		gflops=(flops/1000000000);
		gflops_avg += gflops;
		flops_avg += flops;



	}
	//----------------For IOPS .......................*/
	if(args -> choice == 1) 
	{
		t2=clock();
		int temp_int_value;
		for(i=0;i<INT_MAX;i++)
		{
			temp_int_value +=1;				// calculating interger operation time
		}
		t2 = clock() - t2;
		double intloop =((double)t2)/CLOCKS_PER_SEC;
		double iops=INT_MAX/((intloop-emptyloop));
		double giops=(iops/1000000000);
		giops_avg += giops;
		iops_avg += iops;
	}
}

int select_num_thread(int no_of_threads)
{

	printf("\n Enter the number of threads:");		// Accepting number of threads from user
	scanf("%d",&no_of_threads);
	return no_of_threads;

}

void operation(int no_of_threads,int choice)		
{
	volatile int i=0;
	pthread_t threads[no_of_threads];

	struct arg_struct args;


	int result;
	for(i = 0; i < no_of_threads; i++)
	{	
		
		args.thread_id = i;
		if(choice==0) {
			args.choice = 0;
		} else 
		{
			args.choice=1;
		}
		result = pthread_create(&threads[i], NULL,calculateBenchMark,(void *)&args);    // Creating Thr
		if (result)
		{
			printf("Error:unable to create thread %d",result);
			
		}
	}

	for(i=0;i<no_of_threads;i++) 
	{
		pthread_join(threads[i],NULL);
	}

	if(choice==0) 
	{

		printf("\nGFLOPS:%f \n",gflops_avg/no_of_threads);
		printf("\nFLOPS:%f \n",flops_avg/no_of_threads);
	}

	if(choice==1)
	{

		printf("\nGIOPS:%f \n",(-(giops_avg/no_of_threads)));
		printf("\nIOPS:%f \n",(-(iops_avg/no_of_threads)));
	}
	pthread_exit(NULL);


}


int main ()
{

	int no_of_threads=0;

	int choice;

	do {
		printf("..Menu...\n");
		printf("1.FLOPS\n");
		printf("2.IOPS\n");
		printf("3.EXIT\n");

		printf("\nEnter your choice :");
		scanf("%d",&choice);


		switch(choice)
		{

		case 1: printf("You have selected FLOPS\n");
		        no_of_threads=select_num_thread(no_of_threads);
		        operation(no_of_threads,0);
			break;


		case 2: printf("You have selected IOPS \n");
			no_of_threads=select_num_thread(no_of_threads);
			operation(no_of_threads,1);
			break;

		case 3:
			break;

		}

	} while(choice<3);

	return 0;

}
