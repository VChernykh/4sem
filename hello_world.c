#include <stdio.h>
#include <omp.h>
int main(int argc, char const *argv[])
{	
    omp_set_num_threads(4);
#pragma omp parallel
	{
		int id = omp_get_thread_num();
		int n_threads = omp_get_num_threads();
		printf("Hello, world!, my num: %d, n threads: %d\n", id, n_threads);
	}
	return 0;
}
