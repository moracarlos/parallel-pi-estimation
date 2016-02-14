#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define REF_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062


int main( int argc, char **argv)
{
	if (argc!=2)
	{
		return 0;
	}else{
		unsigned long long int AREAS = atol(argv[1]);
		double mStep = 1/(double)AREAS;
		
		int comm_size;
		int my_rank;

		MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
		MPI_Barrier(MPI_COMM_WORLD);
		double start = MPI_Wtime();

		unsigned long long int initialArea = AREAS/comm_size*my_rank; //position in partition
		unsigned long long int finalArea;
		if (my_rank!=comm_size-1)
		{
	    	finalArea = AREAS/comm_size*(my_rank+1)-1;
		}else{
	    	finalArea = AREAS/comm_size*(my_rank+1);
		}

		printf("Initial %llu final %llu\n", initialArea, finalArea );

		double localSum = 0;
		double x;
		unsigned long long int j =0;
		for (j=initialArea; j<finalArea; j++)
			{
	    	x = (j+0.5)*mStep;
	    	localSum+=4.0/(1.0+x*x);
		}

		double sum=0;

		MPI_Reduce(&localSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
		double end = MPI_Wtime();

		if (my_rank==0)
		{
		 	double PI = sum*mStep;
		 	printf("PI Estimation %.16f\n", PI);
			printf("Error measured %.16f\n", REF_PI-PI);
			printf("Time %.16f\n", end-start);
		}


		MPI_Finalize();
		return 0;
	}
}

