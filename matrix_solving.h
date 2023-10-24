#pragma once

// ld-разложение
void ld_decomposition(double** LD, int n);

// решение разложенной ld-матрицы
void ld_solve(double** LD, double* values, int n, double* result);
