#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUM 999999999
#define FILE_SIZE 15
#warning "Please add -lm for compilation (pow() used)"

int main(int argc, char *argv[])
{
        int rank, proc_num, len, number;
        double start, end = 0;
        char * in_file;
        char * out_file;

        in_file = (char*)calloc(FILE_SIZE, sizeof(char));
        out_file = (char*)calloc(FILE_SIZE, sizeof(char));
        FILE* in;
        FILE* out;
        int* n1;
        int* n2;

        int block_num, block_size, ost;
        MPI_Status Status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
        int i;
        int* result;
        if(rank == 0){
                sscanf(argv[1], "%s", in_file);
                in = fopen(in_file, "r");
                sscanf(argv[2], "%s", out_file);
                out = fopen(out_file, "w");
                fscanf(in, "%d", &len);
                number = len / 9;
                n1 = (int*)calloc(number, sizeof(int));
                n2 = (int*)calloc(number,  sizeof(int));
                for(i = 0; i < number; i++)
                        fscanf(in, "%9d", &n1[i]);
		for(i = 0; i < number; i++)
                        fscanf(in, "%9d", &n2[i]);
                float flows = proc_num - 1;
                block_size = number / (pow(flows, 1.5));
                if(block_size <= 0 )
                        block_size = 1;
                block_num = number / block_size;
                ost = number % block_size;
        }
        MPI_Bcast(&ost, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&block_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&block_num, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        int* rbuf1;
        int* rbuf2;
        int* result0;
        int buf, worker = 1;
        int* cur_sol1;
        int* cur_sol2;
        int* is_spec;
        int* razr1;
        int* razr2;
        int* sol1;
        int* sol2;
        int wr;
        if(rank == 0){
                cur_sol1 = (int*)calloc(number, sizeof(int));
                cur_sol2 = (int*)calloc(number, sizeof(int));
                is_spec = (int*)calloc((block_num + 1),  sizeof(int));
                razr1 = (int*)calloc((block_num + 1),  sizeof(int));
                razr2 = (int*)calloc(block_num, sizeof(int));
                start = MPI_Wtime();
                int flag = 0;
                for(i = 0; i < block_num; i++){
                        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        if(Status.MPI_TAG == 2){
                                flag++;
                                worker = Status.MPI_SOURCE;
                                MPI_Recv(&wr, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                is_spec[wr] = 0;
                                MPI_Recv(&cur_sol1[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                MPI_Recv(&razr1[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                if(Status.MPI_TAG == 7){
                                        is_spec[wr] = 1;
                                        MPI_Recv(&cur_sol2[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                        MPI_Recv(&razr2[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                }
                                MPI_Recv(&buf, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

                        }
                        worker = Status.MPI_SOURCE;
                        MPI_Send(&i, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
                        MPI_Send(&n1[block_size * i], block_size, MPI_INT, worker, 1, MPI_COMM_WORLD);
                        MPI_Send(&n2[block_size * i], block_size, MPI_INT, worker, 1, MPI_COMM_WORLD);
                }
                int* ostbuf = (int*)calloc(ost, sizeof(int));
                ostbuf[ost - 1] = n1[block_num * block_size + ost - 1] + n2[block_size * block_num + ost - 1];
                for(i = ost - 2; i >= 0; i--){
                        ostbuf[i] = n1[block_num * block_size + i] + n2[block_num * block_size + i];
                        if(ostbuf[i + 1] > MAX_NUM)
                                ostbuf[i] += 1;
                }
                is_spec[block_num] = 0;
                razr1[block_num] = 0;
                for(i = 1; i < proc_num; i++)
                        MPI_Send(&buf, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
                for(i = 0; i < block_num - flag; i++){
                        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        worker = Status.MPI_SOURCE;
                        MPI_Recv(&wr, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        is_spec[wr] = 0;
                        MPI_Recv(&cur_sol1[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        MPI_Recv(&razr1[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        if(Status.MPI_TAG == 7){
                                is_spec[wr] = 1;
                                MPI_Recv(&cur_sol2[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                                MPI_Recv(&razr2[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        }
                        MPI_Recv(&buf, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                }
                int j;
                result0 = (int*)calloc(number,  sizeof(int));
                if(ost != 0){
                        for(i = 0; i < ost; i++)
                                result0[block_size * block_num + i] = ostbuf[i];
                        if(ostbuf[0] > MAX_NUM)
                                razr1[block_num] = 1;
                }
                for(i = block_num - 1; i >= 0; i--){
                        if(is_spec[i] == 1){
                                if((is_spec[i + 1] == 1) || (is_spec[i + 1] != 1 && razr1[i + 1] == 1)){
                                        for(j = 0; j < block_size; j++)
                                                result0[i * block_size + j] = cur_sol2[i * block_size + j];
                                }
                                else{
                                        for(j = 0; j < block_size; j++)
                                                result0[i * block_size + j] = cur_sol1[i * block_size + j];
                                }
                        }
                        else{
                                for(j = 0; j < block_size; j++)
                                        result0[i * block_size + j] = cur_sol1[i * block_size + j];
                                if(is_spec[i+1] == 1)
                                        result0[(i + 1) * block_size - 1] += razr2[i+1];
                                else
                                        result0[(i + 1) * block_size - 1] += razr1[i+1];
                        }
                }
                for(i = 0; i < number; i++){
                        if(result0[i] <= MAX_NUM)
                                fprintf(out, "%d", result0[i]);
                        else{
                                if(i != 0)
                                        fprintf(out, "%09d", result0[i] - MAX_NUM - 1);
                                else
                                        fprintf(out, "%d", result0[0]);
                        }
                }
        }
        else{
                rbuf1 = (int*)calloc(block_size,  sizeof(int));
                rbuf2 = (int*)calloc(block_size,  sizeof(int));
                sol1 = (int*)calloc(block_size,  sizeof(int));
                sol2 = (int*)calloc(block_size,  sizeof(int));
                int razr1, razr2, row_num;
                while(1==1){
                        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                        MPI_Recv(&row_num, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        if(Status.MPI_TAG != 1)
                                break;
                        MPI_Recv(rbuf1, block_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        MPI_Recv(rbuf2, block_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
                        if(Status.MPI_TAG == 1){
                                razr1 = 0;
                                sol1[block_size - 1] = rbuf1[block_size - 1] + rbuf2[block_size - 1];
                                if(sol1[block_size - 1] != MAX_NUM){
                                        for(i = block_size - 2; i >= 0; i--){
                                                sol1[i] = rbuf1[i] + rbuf2[i];
                                                if(sol1[i+1] > MAX_NUM)
                                                        sol1[i] += 1;
                                        }
                                        if(sol1[0] > MAX_NUM)
                                                razr1 = 1;
                                        MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
                                        MPI_Send(&row_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
                                        MPI_Send(sol1, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
                                        MPI_Send(&razr1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                                }
                                else{
                                        razr1, razr2 = 0;
                                        sol2[block_size - 1] = sol1[block_size - 1] + 1;
                                        for(i = block_size - 2; i >= 0; i--){
                                                sol1[i] = rbuf1[i] + rbuf2[i];
                                                if(sol1[i+1] > MAX_NUM)
                                                        sol1[i] += 1;
                                                sol2[i] = rbuf1[i] + rbuf2[i];
                                                if(sol2[i+1] > MAX_NUM)
                                                        sol2[i] += 1;
                                        }
                                        if(sol1[0] > MAX_NUM)
                                                razr1 = 1;
                                        if(sol2[0] > MAX_NUM)
                                                razr2 = 1;
                                        MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
                                        MPI_Send(&row_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
                                        MPI_Send(sol1, block_size, MPI_INT, 0, 7, MPI_COMM_WORLD);
                                        MPI_Send(&razr1, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
                                        MPI_Send(sol2, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
                                        MPI_Send(&razr2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                                }
                        }
                        else
                                break;
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
