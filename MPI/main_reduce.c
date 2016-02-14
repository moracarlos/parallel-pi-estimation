#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define REF_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

int main(int argc, char **argv)
{

	if (argc<2){
		return 0;
	}else{
		int my_rank;
		int comm_size;

		MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
		int number_of_tosses = atol(argv[1]);
		int number_in_circle = 0;
		double distance_squared, pi_estimate;
		double x,y;
		int ini_toss, final_toss, toss;
		ini_toss = number_of_tosses/(comm_size) * (my_rank);
		final_toss =  (my_rank==comm_size-1) ?  number_of_tosses :	number_of_tosses/(comm_size) * (my_rank+1) ;

		for (toss = ini_toss; toss < final_toss; toss++) {

			x =((rand()%20)/(double)10)-1;
			y= ((rand()%20)/(double)10)-1;
			distance_squared = x*x + y*y;
			if (distance_squared <= 1) number_in_circle++;
		}

		int global_number_in_circle;	

		MPI_Reduce(&number_in_circle, &global_number_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if (my_rank==0)
		{
			pi_estimate = 4*number_in_circle/(( double ) number_of_tosses);			
			printf("PI Value %.16f\n", pi_estimate);
		}

		MPI_Finalize();
	}
	return 0;
}	

