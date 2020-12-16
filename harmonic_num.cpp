#include "omp.h"
#include <iostream>
#include <unistd.h>


int main(int argc, char **argv) {
    int num_threads = std::stoi(argv[1]);
    long int N = std::stoi(argv[2]);
    omp_set_num_threads(num_threads);
    double result = 0;
    double start = omp_get_wtime();
    omp_lock_t mutex;
    omp_init_lock(&mutex);

#pragma omp parallel{
        double sum = 0;

#pragma omp for
        for (int i = 1; i <= N; ++i)
            sum += 1 / (double) i;

        omp_set_lock(&mutex);
        result += sum;
        omp_unset_lock(&mutex);
    };

    omp_destroy_lock(&mutex);
    double finish = omp_get_wtime();
    double parr = omp_get_wtime() - start;
    std::cout << "Parallel\n" << "Counted: " << result << "\nWorking time: " << finish - start << std::endl;
    result = 0;
    start = omp_get_wtime();
    for (int i = 1; i <= N; ++i) {
        result += 1 / (double) i;
    }
    finish = omp_get_wtime();
    double sequence = finish - start;
    std::cout << "\nSequence\n" << "Counted: " << result << "\nWorking time: " << finish - start << std::endl;
    std::cout << "\nBoost: " << parr/sequence << std::endl << "Num processes: " << num_threads << std::endl;
    return 0;
}
