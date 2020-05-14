#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include <malloc.h> 



int main(int argc, char *argv[])
{
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
	
	int block_num;
	int block_size;
	int rest;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

	if(my_rank == 0)
	{
		if(argc != 3)
		{
			printf("Programm requires two file names!\n");
			MPI_Finalize();
			return 0;
		}
		sscanf(argv[1], "%s", &inpf);
		if((fr = fopen(inpf, "r")) == NULL)
		{
			printf("Can't open first file!\n");
		}
		sscanf(argv[2], "%s", &outf);
		if((fw = fopen(outf, "w")) == NULL)
		{
			printf("Can't open second file!\n");
		}
	
		fscanf(fr, "%d", &num_len);
		count = num_len / 9;

		num1 = (int*)malloc(count * sizeof(int));
		num2 = (int*)malloc(count * sizeof(int));
		for(i = 0; i < count; i++)
		{
			fscanf(fr, "%9d", &num1[i]);
		}
		
		for(i = 0; i < count; i++)
		{
			fscanf(fr, "%9d", &num2[i]);
		}
		
		block_size = count / (proc_num - 1) / sqrt(proc_num - 1);
		if(block_size < 1)
		{
			block_size = 1;
		}
		block_num = count / block_size;
		rest = count % block_size;
	}
	MPI_Bcast(&rest, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&block_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&block_num, 1, MPI_INT, 0, MPI_COMM_WORLD);	

	int* rbuf1;
	int* rbuf2;
	int* result0;
	int buf = 0;
	int worker = 0;
	int* cur_sol1;
	int* cur_sol2;
	int* is_spec;
	int* razr1;
	int* razr2;
	int* my_sol1;	
	int* my_sol2;
	int wr;
	MPI_Barrier(MPI_COMM_WORLD);
	if(my_rank == 0)
	{
		int already = 0;
		cur_sol1 = (int*)malloc(count * sizeof(int));
		cur_sol2 = (int*)malloc(count * sizeof(int));
		is_spec = (int*)malloc((block_num + 1) * sizeof(int));
		razr1 = (int*)malloc((block_num + 1) * sizeof(int));
		razr2 = (int*)malloc(block_num * sizeof(int));
		start_time = MPI_Wtime();
		for(i = 0; i < block_num; i++)
		{
			MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			if(Status.MPI_TAG == 2)
			{
				already++;
				worker = Status.MPI_SOURCE;
				MPI_Recv(&wr, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				is_spec[wr] = 0;
				MPI_Recv(&cur_sol1[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				MPI_Recv(&razr1[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				if(Status.MPI_TAG == 7)
				{
					is_spec[wr] = 1;
					MPI_Recv(&cur_sol2[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
					MPI_Recv(&razr2[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				}
				MPI_Recv(&buf, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				
			}
			
			worker = Status.MPI_SOURCE;
			MPI_Send(&i, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
			MPI_Send(&num1[block_size * i], block_size, MPI_INT, worker, 1, MPI_COMM_WORLD);
			MPI_Send(&num2[block_size * i], block_size, MPI_INT, worker, 1, MPI_COMM_WORLD); 
		}
		int* restbuf = (int*)malloc(rest * sizeof(int));
		restbuf[rest - 1] = num1[block_num * block_size + rest - 1] + num2[block_size * block_num + rest - 1];
		for(i = rest - 2; i >= 0; i--)
		{
			restbuf[i] = num1[block_num * block_size + i] + num2[block_num * block_size + i];
			if(restbuf[i + 1] > 999999999)
			{
				restbuf[i] += 1;
			}
		}
		is_spec[block_num] = 0;
		razr1[block_num] = 0;
		for(i = 1; i < proc_num; i++)
		{
			MPI_Send(&buf, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
		}
		for(i = 0; i < block_num - already; i++)
		{
			MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			worker = Status.MPI_SOURCE;
			MPI_Recv(&wr, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			is_spec[wr] = 0;
			MPI_Recv(&cur_sol1[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			MPI_Recv(&razr1[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			if(Status.MPI_TAG == 7)
			{
				is_spec[wr] = 1;
				MPI_Recv(&cur_sol2[wr * block_size], block_size, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				MPI_Recv(&razr2[wr], 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			}
			MPI_Recv(&buf, 1, MPI_INT, worker, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
		}
		result0 = (int*)malloc(count * sizeof(int));
		if(rest != 0)
		{
			for(i = 0; i < rest; i++)
			{
				result0[block_size * block_num + i] = restbuf[i];
			}
			if(restbuf[0] > 999999999)
			{
				razr1[block_num] = 1;
			}
		}
		int j;
		for(i = block_num - 1; i >= 0; i--)
		{
			if(is_spec[i] == 1)
			{
				if((is_spec[i + 1] == 1) || (is_spec[i + 1] != 1 && razr1[i + 1] == 1))
				{
					for(j = 0; j < block_size; j++)
					{
						result0[i * block_size + j] = cur_sol2[i * block_size + j];
					}
				}
				else
				{
					for(j = 0; j < block_size; j++)
					{
						result0[i * block_size + j] = cur_sol1[i * block_size + j];
					}
				}
			}
			else
			{
				for(j = 0; j < block_size; j++)
				{
					result0[i * block_size + j] = cur_sol1[i * block_size + j];
				}
				if(is_spec[i+1] == 1)
				{
					result0[(i + 1) * block_size - 1] += razr2[i+1];
				}
				else
				{
						result0[(i + 1) * block_size - 1] += razr1[i+1];
				}
			}
		}

		for(i = 0; i < count; i++)
		{
			if(result0[i] < 1000000000)
			{
				fprintf(fw, "%d", result0[i]);
			}
			else
			{
				if(i != 0)
				{
					fprintf(fw, "%09d", result0[i] - 1000000000);
				}
				else
				{
					fprintf(fw, "%d", result0[0]);
				}
			}
		}

	}
	else
	{
		int row_num;
		rbuf1 = (int*)malloc(block_size * sizeof(int));
		rbuf2 = (int*)malloc(block_size * sizeof(int));
		my_sol1 = (int*)malloc(block_size * sizeof(int));
		my_sol2 = (int*)malloc(block_size * sizeof(int));
		int razr1;
		int razr2;
	
		while(1)
		{
			MPI_Send(&my_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			MPI_Recv(&row_num, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			if(Status.MPI_TAG != 1)
			{
				break;
			}
			MPI_Recv(rbuf1, block_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			MPI_Recv(rbuf2, block_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			if(Status.MPI_TAG == 1)
			{
				razr1 = 0;
				my_sol1[block_size - 1] = rbuf1[block_size - 1] + rbuf2[block_size - 1];
				
				if(my_sol1[block_size - 1] != 999999999)
				{
					for(i = block_size - 2; i >= 0; i--)
					{
						my_sol1[i] = rbuf1[i] + rbuf2[i];
						if(my_sol1[i+1] > 999999999)
						{
							my_sol1[i] += 1;
						}
					}
					if(my_sol1[0] > 999999999)
					{
						razr1 = 1;
					}
					MPI_Send(&my_rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
					MPI_Send(&row_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
					MPI_Send(my_sol1, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
					MPI_Send(&razr1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				}
				else
				{
					razr1 = 0;
					razr2 = 0;
					my_sol2[block_size - 1] = my_sol1[block_size - 1] + 1;
					for(i = block_size - 2; i >= 0; i--)
					{
						my_sol1[i] = rbuf1[i] + rbuf2[i];
						if(my_sol1[i+1] > 999999999)
						{
							my_sol1[i] += 1;
						}	
						my_sol2[i] = rbuf1[i] + rbuf2[i];
						if(my_sol2[i+1] > 999999999)
						{
							my_sol2[i] += 1;
						}
					}
					if(my_sol1[0] > 999999999)
					{
						razr1 = 1;
					}
					if(my_sol2[0] > 999999999)
					{
						razr2 = 1;
					}
					MPI_Send(&my_rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
					MPI_Send(&row_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
					MPI_Send(my_sol1, block_size, MPI_INT, 0, 7, MPI_COMM_WORLD);
					MPI_Send(&razr1, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
					MPI_Send(my_sol2, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
					MPI_Send(&razr2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				}	

			}
			else
			{
				break;
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(my_rank == 0)
	{
		end_time = MPI_Wtime();
		printf("Time: %f\n", end_time - start_time); 
		fclose(fr);
		fclose(fw);
	}
	MPI_Finalize();
	return 0;
}
