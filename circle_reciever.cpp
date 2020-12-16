#include "omp.h"
#include <iostream>

int main(int argc, char** argv) {

    int proc_id = 0;
    double sum = 0;
    int num_threads = std::stoi(argv[1]);
    omp_set_num_threads(num_threads);
    omp_lock_t mutex;
    omp_init_lock(&mutex);

#pragma omp parallel{
        while (proc_id != omp_get_thread_num());
        omp_set_lock(&mutex);
        sum += omp_get_thread_num();
        std::cout << "Process " << omp_get_thread_num() << ": " << sum << std::endl;
        omp_unset_lock(&mutex);
        ++proc_id;
    }
    return 0;
}
