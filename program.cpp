#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>

#include "iomatrix.h"

double** ld(double** A, int n) {
    double** LD = copy(A, n, n);

    for (int j = 0; j < n; ++j) {

        double* v = zeros(n + 1);

        for (int i = 0; i < j; ++i) {
            v[i] = LD[j][i] * LD[i][i];
        }

        double sum = 0.0;
        for (int i = 0; i < j; ++i) {
            sum += LD[j][i] * v[i];
        }
        v[j] = LD[j][j] - sum;
        LD[j][j] = v[j];

        for (int i = j + 1; i < n; ++i) {
            double sub_sum = 0.0;
            for (int k = 0; k < j; ++k) {
                sub_sum += LD[i][k] * v[k];
            }
            LD[i][j] = (LD[i][j] - sub_sum) / v[j];
        }
    }

    return LD;
}

double* ld_solve(double** A, int n, double* b) {
    double** LD = ld(A, n);

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            b[i] -= LD[i][j] * b[j];
        }
    }

    for (int i = 0; i < n; ++i) {
        b[i] /= LD[i][i];
    }

    for (int i = n - 2; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            b[i] -= LD[j][i] * b[j];
        }
    }

    delete LD;

    return b;
}

double fcfs(int nmatrices) {
    if (!std::filesystem::exists("fcfs")) {
        std::filesystem::create_directory("fcfs");
    }
    
    std::cout << "First Come, First Served" << std::endl;

    std::chrono::duration<double> duration{ 0 };
    for (int i = 0; i < nmatrices; i++) {
        int n = 0, m = 0;
        double** matr = readmatrix("matrices/" + std::to_string(i) + ".txt", &n, &m);
        double* arr = readarr("values/" + std::to_string(i) + ".txt", &n);

        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        double* result = ld_solve(matr, n, arr);
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        writearr("fcfs/" + std::to_string(i) + ".txt", result, n);

        std::chrono::duration<double> localduration = end - start;
        //std::cout << "Matrice " << i + 1 << " solved by " << localduration.count() << " seconds" << std::endl;
        duration += localduration;
        delete matr, arr, result;
    }
    std::cout << "Total time is " << duration.count() << " seconds" << std::endl;
    return duration.count();
}

void compute_ld(double** matr, double* arr, int n, int i, int *completed, std::chrono::duration<double>* total)
{
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    double* result = ld_solve(matr, n, arr);
    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    writearr("guaranted/" + std::to_string(i) + ".txt", result, n);
    std::chrono::duration<double> localduration = end - start;

    //std::cout << "Matrice " << i + 1 << " solved by " << localduration.count() << " seconds" << std::endl;
    *total += localduration;
    (*completed)++;

    delete matr, arr, result;
}

double guaranted(int nmatrices) {
    if (!std::filesystem::exists("guaranted")) {
        std::filesystem::create_directory("guaranted");
    }

    std::cout << "Guaranted planning" << std::endl;

    int threadcompleted = 0;
    std::chrono::duration<double> total{ 0 };
    for (int i = 0; i < nmatrices; i++) {
        int n = 0, m = 0;
        double** matr = readmatrix("matrices/" + std::to_string(i) + ".txt", &n, &m);
        double* arr = readarr("values/" + std::to_string(i) + ".txt", &n);

        std::thread th = std::thread(compute_ld, matr, arr, n, i, &threadcompleted, &total);
        th.detach();
    }
    while (threadcompleted < nmatrices);
    std::cout << "Total time is " << total.count() << " seconds" << std::endl;
    return total.count();
}

int main() {
    int n = 10;
    genwritematrices(n);
    fcfs(n);
    guaranted(n);
    std::cout << "Press enter to continue..." << std::endl;
    getchar();
}
