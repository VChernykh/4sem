#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define MAX_NUM 999999999
#define FILE_SIZE 15



int  main(int argc, char *argv[]){

        //files
        char* in_file;
        char* out_file;
        in_file = (char*)calloc(FILE_SIZE, sizeof(char));
        out_file = (char*)calloc(FILE_SIZE, sizeof(char));
        FILE* in;
        FILE* out;

        int rank, proc_num, len, number, share;
        double start, end  = 0;
        int* n1;
        int* n2;
        int* summa;

        MPI_Status Status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &proc_num);


        int i;
        if(rank == 0){
                sscanf(argv[1], "%s", in_file);
                in = fopen(in_file, "r");
                sscanf(argv[2], "%s", out_file);
                out = fopen(out_file, "w");
                fscanf(in, "%d", &len);
                number = len / 9;
                share = number / (proc_num - 1);
                n1 = (int*)calloc(number, sizeof(int));
                n2 = (int*)calloc(number,  sizeof(int));
                for(i = 0; i < number; i++)
                        fscanf(in, "%9d", &n1[i]);
                for(i = 0; i < number; i++)
                        fscanf(in, "%9d", &n2[i]);
        }
        int* summa_init;

        MPI_Bcast(&share, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0){
                summa_init = (int*)calloc(number, sizeof(int));
                start = MPI_Wtime();


                for(i = 1; i < proc_num; i++){
                        MPI_Send(&n1[(i - 1) * share], share, MPI_INT, i, 0, MPI_COMM_WORLD);
                        MPI_Send(&n2[(i - 1) * share], share, MPI_INT, i, 0, MPI_COMM_WORLD);
                }

                for(i = 1; i < proc_num; i++)
                        MPI_Recv(&summa_init[(i - 1) * share], share, MPI_INT, i, 0, MPI_COMM_WORLD, &Status);
                for(i = 0; i < number; i++){
                        if(summa_init[i] <= MAX_NUM)
                                fprintf(out, "%d", summa_init[i]);
                        else{
                                if(i != 1)
                                        fprintf(out, "%09d", summa_init[i] - MAX_NUM - 1);
                                else
                                        fprintf(out, "%d", summa_init[0]);
                        }
                }

        }
        else{
                int* buf1;
                int* buf2;
                buf1 = (int*)calloc(share, sizeof(int));
                buf2 = (int*)calloc(share, sizeof(int));
                MPI_Recv(buf1, share, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
                MPI_Recv(buf2, share, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);

                summa = (int*)calloc(share, sizeof(int));
                summa[share - 1] = buf1[share - 1] + buf2[share - 1];
                if (summa[share - 1] != MAX_NUM && share >= 2){
                        for(i = share - 2; i >= 0; i--){
                                summa[i] = buf1[i] + buf2[i];
                                if(summa[i + 1] > MAX_NUM)
                                        summa[i] += 1;
                        }
                        int flag, flag_pre = 0;
                        if(summa[0] > MAX_NUM)
                                flag = 1;
                        if(rank != 1)
                                MPI_Send(&flag, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

                        if(rank != proc_num - 1)
                                MPI_Recv(&flag_pre, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &Status);
                        summa[share - 1] += flag_pre;
                        MPI_Send(summa, share, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
                else{
                        int* summa_end = (int*)calloc(share, sizeof(int));
                        summa_end[share - 1] = summa[share - 1] + 1;
                        if(share >= 2){
                                for(i = share - 2; i >= 0; i--){
                                        summa[i] = buf1[i] + buf2[i];
                                        if(summa[i+1] > MAX_NUM)
                                                summa[i] += 1;
                                        summa_end[i] = buf1[i] + buf2[i];
                                        if(summa_end[i + 1] > MAX_NUM)
                                                summa_end[i] += 1;
                                }
                        }
                        int flag_pre = 0;
                        if(rank != proc_num - 1)
                                MPI_Recv(&flag_pre, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &Status);
                        if(flag_pre == 0){
                                MPI_Send(summa, share,  MPI_INT, 0, 0, MPI_COMM_WORLD);
                                int flag = 0;
                                if(summa[0] > MAX_NUM)
                                        flag = 1;
                                if(rank != 1)
                                        MPI_Send(&flag, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                        }
                        else{
                                MPI_Send(summa_end, share, MPI_INT, 0, 0, MPI_COMM_WORLD);
                                int flag = 0;
                                if(summa_end[0] > MAX_NUM)
                                        flag = 1;
                                if(rank != 1)
                                        MPI_Send(&flag, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                        }
                }

        }

        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0){
                end = MPI_Wtime();
		printf("%f\n", end - start);
                fclose(in); fclose(out);
        }
        MPI_Finalize();
        return 0;
}
 
