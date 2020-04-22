
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#define PI 3.14159265

int main(int argc, char *argv[])
{
        int rank, proc_num;
        int i, each, size;
        int* sbuf;
        int* rbuf;
        double start, finish, total_sum = 0;     

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

        double item = 0;
        rbuf = (int*)calloc(each,  sizeof(int));
        double* summa_buf = (double*)calloc(proc_num, sizeof(double));

        if(rank == 0){
                sscanf(argv[1], "%d", &size);
                each = size / proc_num + 1;
                sbuf = (int*)calloc(each * proc_num, sizeof(int));

                for(i = 1; i <= each * proc_num; i++){
                        if(i > size)
                                sbuf[i - 1] = 0;
                        else
                                sbuf[i - 1] = i;
                }
        }
        MPI_Scatter(sbuf, each, MPI_INT, rbuf, each, MPI_INT, 0, MPI_COMM_WORLD);
//time
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0)
                start = MPI_Wtime();

        for(i = each - 1; i >= 0; i--){
                if(rbuf[i] != 0)
                        item += 6 /(PI * PI * rbuf[i] * rbuf[i]);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(&item, 1, MPI_DOUBLE, summa_buf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if(rank == 0)
                finish = MPI_Wtime();
//stop;

        if(rank == 0){
                for(i = proc_num - 1; i >= 0; i--)
                        total_sum += summa_buf[i];
                printf("Total sum = %lf\nTime = %lf\n", total_sum, (finish - start));
        }
        MPI_Finalize();
        return 0;
}
