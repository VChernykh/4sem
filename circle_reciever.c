#include <stdio.h>
#include <omp.h>
int main(int argc, char const *argv[])
{
        // scp -P 52960 /Users/andreyandriyaynen/CLionProjects/2_sem_parprog/series.c s71709@remote.vdi.mipt.ru:/home/s71709/2_sem_init
        // gcc -fopenmp -std=c99 -o series series.c
        int n_threads = atoi(argv[1]);
        omp_set_num_threads(n_threads);
        int res = 0;
        int thread_id = 0;
#pragma omp parallel
        {
        #pragma omp for ordered
                for (int i = 0; i < omp_get_num_threads(); i++)
                {
                #pragma omp ordered
                        {
                                thread_id = omp_get_thread_num();
                                res += omp_get_thread_num();
                                printf("Thread[%d]: sum = %d\n", thread_id, res);
                        }
                }
        };
        return 0;
}
