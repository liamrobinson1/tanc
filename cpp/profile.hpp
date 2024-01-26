#include <iostream>
#include <chrono>

// Global variable to store the start time
std::chrono::time_point<std::chrono::high_resolution_clock> tic_start;

// Function to mimic MATLAB's tic
void tic() {
    tic_start = std::chrono::high_resolution_clock::now();
}

// Function to mimic MATLAB's toc
double toc(bool print = true) {
    auto toc_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = toc_end - tic_start;
    double dt = elapsed.count();
    
    if (print) {
        if (dt < 1e-3) {
            std::cout << "Time elapsed: " << dt * 1e6 << " us" << std::endl;
        } else if (dt < 1) {
            std::cout << "Time elapsed: " << dt * 1e3 << " ms" << std::endl;
        } else {
            std::cout << "Time elapsed: " << dt << " s" << std::endl;
        }
    }
    return dt;
}