#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	int my_rank;
	int proc_num;
	int i;
	int num_len;
	int count;
	double start_time = 0;
	double end_time = 0;
	char inpf[10];
	char outf[10];
	FILE* fr;
	FILE* fw;
	int* num1;
	int* num2;
	int* result;
	int b_per_proc;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

	if(my_rank == 0){
		if(argc != 3){
			printf("Programm requires two file names!\n");
			MPI_Finalize();
			return 0;
		}
		sscanf(argv[1], "%s", &inpf);
		if((fr = fopen(inpf, "r")) == NULL){
			printf("Can't open first file!\n");
		}
		sscanf(argv[2], "%s", &outf);
		if((fw = fopen(outf, "w")) == NULL){
			printf("Can't open second file!\n");
		}
		
		fscanf(fr, "%d", &num_len);
		count = num_len / 9;

		num1 = (int*)calloc(count, sizeof(int));
		num2 = (int*)calloc(count, sizeof(int));
		for(i = 0; i < count; i++){
			fscanf(fr, "%9d", &num1[i]);
		}
		
		for(i = 0; i < count; i++){
			fscanf(fr, "%9d", &num2[i]);
		}
	
		b_per_proc = count / (proc_num - 1);
	}
	MPI_Bcast(&b_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int* rbuf1;
	int* rbuf2;
	int* result0;
	MPI_Barrier(MPI_COMM_WORLD);
	if(my_rank == 0){
		start_time = MPI_Wtime();
		for(i = 1; i < proc_num; i++){
			MPI_Send(&num1[b_per_proc*(i-1)], b_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&num2[b_per_proc*(i-1)], b_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
			
		result0 = (int*)calloc(count, sizeof(int));
		for(i = 1; i < proc_num; i++){
			MPI_Recv(&result0[b_per_proc*(i-1)], b_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD, &Status);
		}
		for(i = 0; i < count; i++){
			if(result0[i] < 1000000000){
				fprintf(fw, "%d", result0[i]);
			}
			else{
				if(i != 0)
					fprintf(fw, "%09d", result0[i] - 1000000000);
				else
					fprintf(fw, "%d", result0[0]);
			}						
		}	

	}
	else{
		rbuf1 = (int*)calloc(b_per_proc, sizeof(int));
		rbuf2 = (int*)calloc(b_per_proc, sizeof(int));
		MPI_Recv(rbuf1, b_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
		MPI_Recv(rbuf2, b_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);

		result = (int*)calloc(b_per_proc, sizeof(int));
		result[b_per_proc - 1] = rbuf1[b_per_proc - 1] + rbuf2[b_per_proc - 1];
		if(result[b_per_proc - 1] != 999999999 && b_per_proc >= 2){
			for(i = b_per_proc - 2; i >= 0; i--){
				result[i] = rbuf1[i] + rbuf2[i];
				if(result[i+1] > 999999999)
					result[i] += 1;
			}
			int flag = 0;
			if(result[0] > 999999999)
				flag = 1;
			if(my_rank != 1)
				MPI_Send(&flag, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
			int prev = 0;
			if(my_rank != proc_num - 1)
				MPI_Recv(&prev, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &Status);
			result[b_per_proc - 1] += prev;
			MPI_Send(result, b_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		else{
			int* resultplus = (int*)calloc(b_per_proc, sizeof(int));
			resultplus[b_per_proc - 1] = result[b_per_proc - 1] + 1;
			if(b_per_proc >= 2){
				for(i = b_per_proc - 2; i >= 0; i--){
					result[i] = rbuf1[i] + rbuf2[i];
					if(result[i+1] > 999999999)
						result[i] += 1;
					resultplus[i] = rbuf1[i] + rbuf2[i];
					if(resultplus[i+1] > 999999999)
						resultplus[i] += 1;
				}
			}
			int prev = 0;
			if(my_rank != proc_num - 1)
				MPI_Recv(&prev, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &Status);
			if(prev == 0){
				MPI_Send(result, b_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
				int flag = 0;
				if(result[0] > 999999999)
					flag = 1;
				if(my_rank != 1)
					MPI_Send(&flag, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
			}
			else{
				MPI_Send(resultplus, b_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
				int flag = 0;
				if(resultplus[0] > 999999999)
					flag = 1;
				if(my_rank != 1)
					MPI_Send(&flag, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
			}
		}

	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(my_rank == 0){
		end_time = MPI_Wtime();
		printf("Time: %f\n", end_time - start_time); 
		fclose(fr);
		fclose(fw);
	}
	MPI_Finalize();
	return 0;
}
