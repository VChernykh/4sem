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
        rbuf = (int*)malloc(k * sizeof(int));
        double item = 0;
        double* sum_buf = (double*)malloc(proc_num * sizeof(double));
        MPI_Scatter(sbuf, k, MPI_INT, rbuf, k, MPI_INT, 0, MPI_COMM_WORLD);

//time measurement
        MPI_Barrier(MPI_COMM_WORLD);
        if(my_rank == 0)
                start_time = MPI_Wtime();

        for(i = k - 1; i >= 0; i--){
        //for(i = 1; i <=  k; i++){     
                if(rbuf[i] != 0)
                        item += 6 / 3.1415926535 / 3.1415926535 / rbuf[i] / rbuf[i];
        }

        MPI_Barrier(MPI_COMM_WORLD);
//

        if(my_rank == 0)
                end_time = MPI_Wtime();

        MPI_Gather(&item, 1, MPI_DOUBLE, sum_buf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if(my_rank == 0){
                for(i = proc_num - 1; i >= 0; i--){
                        result += sum_buf[i];
                }
        }

        MPI_Finalize();

        if(my_rank == 0){
                printf("Result: %lf\n", result);
                printf("Time: %lf\n", end_time - start_time);
        }
        return 0;
}
                                             
