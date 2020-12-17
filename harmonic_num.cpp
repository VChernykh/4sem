#include <stdio.h>
#include <omp.h>


int main(int argc, char const *argv[])
{
	// scp -P 52960 /Users/andreyandriyaynen/CLionProjects/2_sem_parprog/series.c s71709@remote.vdi.mipt.ru:/home/s71709/2_sem_init
	// gcc -fopenmp -std=c99 -o series series.c
	//first arg is num of threads
	int n_threads = atoi(argv[1]);

	char *ptr;
	long int N = strtol(argv[2], &ptr, 10);
	omp_set_num_threads(n_threads);
	double res = 0;
	double start_time = omp_get_wtime();
	omp_lock_t mutex;
    	omp_init_lock(&mutex);

#pragma omp parallel
	{
		double sum = 0;

	#pragma omp for
		for (int i = 1; i < N; i++)
		{
			sum += 1 / (double) i;
		}

		omp_set_lock(&mutex);
		res += sum;
		omp_unset_lock(&mutex);
	};
	omp_destroy_lock(&mutex);
	double duration_par = omp_get_wtime() - start_time;
	printf("Number of threads= %d\n", n_threads);
	printf("Parallel: n=%ld  Time=%f Result=%f\n", N,duration_par,res);
	res = 0;
	start_time = omp_get_wtime();
	for (int i = 1; i < N; i++){
		res += 1 / (double) i;
	}

	double duration_seq = omp_get_wtime() - start_time;
	printf("Sequence: n=%ld  Time=%f\n, Result=%f", N, duration_seq, res);
	printf("Acceleration= %f\n", duration_seq / duration_par);

	return 0;
}
