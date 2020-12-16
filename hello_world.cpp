#include "omp.h"
#include <iostream>

int main(int argc, char** argv){
    omp_set_num_threads(3);
#pragma omp parallel{
        std::cout << "Hello world!" << std::endl;
    }
}
