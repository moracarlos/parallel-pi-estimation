#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define REF_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062


void Hello(void);

int thread_count = 1;

int main(int argc, char **argv)
{
	int AREAS = strtol(argv[1], NULL, 10);
	thread_count = strtol(argv[2], NULL, 10);
	double mStep = 1/(double)AREAS;
	double sum=0;
    int i;
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	#pragma omp parallel for num_threads(thread_count) default(none) reduction(+:sum) private(i) shared(mStep, AREAS, thread_count)
	for (i=0; i<thread_count; i++)
	{

    	int initialArea = AREAS/thread_count*i; //position in partition
    	int finalArea;
    	if (i!=thread_count-1)
    	{
        	finalArea = AREAS/thread_count*(i+1)-1;
    	}else{
        	finalArea = AREAS/thread_count*(i+1);
    	}
    
		double localSum = 0;
    	double x;
		int j =0;
    	for (j=initialArea; j<finalArea; j++)
   		{
        	x = (j+0.5)*mStep;
        	localSum+=4.0/(1.0+x*x);
    	}

	sum+=localSum;
	}
    double PI = sum*mStep;

	gettimeofday(&stop, NULL);
	
	printf("Time in miliseconds %lu\n", stop.tv_usec - start.tv_usec);	

	//double seconds = difftime(end, start);
	//printf("Time %.16f\n", seconds);
	printf("Pi value %.16f\n", PI);
	printf("Error measured %.16f\n", REF_PI-PI);
	return 0;
}

