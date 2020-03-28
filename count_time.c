Результаты:
1. MPI_Bcast
Time = 0.000006
Error = 0.000001

2. MPI_Scatter
Time = 0.000005
Error = 0.000001

3. MPI_Reduce
Time = 0.000005
Error = 0.000001

4. MPI_Gather
Time = 0.000005
Error = 0.000001

Код:
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){
        int rank = 0, size = 0;
        double start_time = 0, end_time = 0;
        int num_iter = 1000, num_proc = 4;
        double timer = 0;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int* sendbuf = (int*)calloc(num_proc * size, sizeof(int));
        int* recvbuf = (int*)calloc(num_proc * size, sizeof(int));
        int i;
        for(i = 0; i < num_iter; i++){
                MPI_Barrier(MPI_COMM_WORLD);
                if(rank == 0){
                        start_time = MPI_Wtime();
                }
		//Выбор функции
                MPI_Scatter(sendbuf, 2, MPI_INT, recvbuf, 2, MPI_INT, 0, MPI_COMM_WORLD);
                //MPI_Gather(recvbuf, 2, MPI_INT, sendbuf, 2, MPI_INT, 0, MPI_COMM_WORLD);
                // MPI_Bcast(sendbuf, 10, MPI_INT, 0, MPI_COMM_WORLD);
                //MPI_Reduce(recvbuf, sendbuf, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);                 

                MPI_Barrier(MPI_COMM_WORLD);
                if (rank == 0){
                        end_time = MPI_Wtime();
                        timer += (end_time - start_time);
                }
        }                                                                               	if(rank == 0){
                printf("Time = %lf\nError = %lf\n", (timer / num_iter), MPI_Wtick());
        }

        MPI_Finalize();
        return 0;
}
