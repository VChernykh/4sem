#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
        int rank, proc_num;
        int size, each;
        double total_sum = 0;
        double start, finish = 0;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

        int* sbuf;
        int* rbuf;
        int i;
        if(rank == 0){
                sscanf(argv[1], "%d", &size);
                if((size % proc_num) == 0){
                        each = size / proc_num;
                }
                else{
                        each = size / proc_num + 1;
                }

                sbuf = (int*)malloc(each * proc_num * sizeof(int));

                for(i = 1; i <= each * proc_num; i++){
                        if(i <= size){
                                sbuf[i - 1] = i;
                        }
                        else{
                                sbuf[i - 1] = 0;
                        }
                }
        }
        rbuf = (int*)malloc(each * sizeof(int));
        double* sum_buf = (double*)malloc(proc_num * sizeof(double));
        MPI_Scatter(sbuf, each, MPI_INT, rbuf, each, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0)
                start = MPI_Wtime();

        double sum = 0;
        for(i = each - 1; i >= 0; i--){
                if(rbuf[i] != 0){
                        sum += 6 / 3.1415926535 / 3.1415926535 / rbuf[i] / rbuf[i];
                }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0)
                finish = MPI_Wtime();

        MPI_Gather(&sum, 1, MPI_DOUBLE, sum_buf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if(rank == 0){
                for(i = proc_num - 1; i >= 0; i--){
                        total_sum += sum_buf[i];
                }
        }

        MPI_Finalize();

        if(rank == 0){
                printf("Time: %lf\nTotal sum: %lf\n", total_sum, finish - start);
        }
        return 0;
}
