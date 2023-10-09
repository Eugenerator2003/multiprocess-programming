#pragma once
#include <string>
#include <random>
constexpr auto MAX_VAL = 200;;

double* zeros(int n);

double** zeros(int n, int m);

double** copy(double** src, int n, int m);

void print(double** matrx, int n, int m);

double* readarr(std::string name, int* n);

double** readmatrix(std::string name, int* n, int* m);

void writematrix(std::string name, double** matr, int n, int m);

void writearr(std::string name, double* arr, int n);

double* genarray(int n = NULL, int maxvalue = MAX_VAL, std::mt19937* random = nullptr);

double** genmatrix(int &n, int &m, int maxvalue = MAX_VAL, std::mt19937* random = nullptr);

void genwritematrices(int count);