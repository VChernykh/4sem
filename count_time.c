#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

int main(int argc, char** argv)
{
	int rank, size = 0;
	double start_time, end_time = 0;
	
	int num_for_proc = 3;

	int num = 100;	
	int i;
	double sum = 0;	

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* sbuf = (int*)malloc(size * num_for_proc * sizeof(int));
	int* rbuf = (int*)malloc(num_for_proc * sizeof(int));

	for(i = 0; i < num; i++)
	{
		//for 4 processes
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == 0)
			start_time = MPI_Wtime();

		//MPI_Bcast(sbuf, 12, MPI_INT, 0, MPI_COMM_WORLD);
		//MPI_Reduce(rbuf, sbuf, 3, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 		
		//MPI_Scatter(sbuf, 3, MPI_INT, rbuf, 3, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Gather(rbuf, 3, MPI_INT, sbuf, 3, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == 0)
		{
			end_time = MPI_Wtime();
			if(i > 10)
			{
				sum += end_time - start_time;
			}
		}
	}
	if(rank == 0)
	{
		printf("Time: %lf\n", sum / (num - 10));
		printf("Delt: %lf\n", MPI_Wtick());
	}
	MPI_Finalize();
	return 0;
}

//Bcast: 0.000007
//Reduc: 0.000006
//Scatt: 0.000006
//Gathe: 0.000006
