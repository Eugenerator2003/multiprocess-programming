#include "matrix_solving.h"

// ld-разложение
void ld_decomposition(double** LD, int n) {
    for (int j = 0; j < n; j++) {

        double* v = new double[n + 1];
        for (int i = 0; i < n; i++) {
            v[i] = 0;
        }

        for (int i = 0; i < j; i++) {
            v[i] = LD[j][i] * LD[i][i];
        }

        double sum = 0.0;
        for (int i = 0; i < j; i++) {
            sum += LD[j][i] * v[i];
        }

        v[j] = LD[j][j] - sum;
        LD[j][j] = v[j];

        for (int i = j + 1; i < n; i++) {
            double sub_sum = 0.0;
            for (int k = 0; k < j; k++) {
                sub_sum += LD[i][k] * v[k];
            }
            LD[i][j] = (LD[i][j] - sub_sum) / v[j];
        }
        delete[] v;
    }
}

// решение разложенной ld-матрицы
void ld_solve(double** LD, double* values, int n, double* result) {

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; ++j) {
            values[i] -= LD[i][j] * values[j];
        }
    }

    for (int i = 0; i < n; i++) {
        values[i] /= LD[i][i];
    }

    for (int i = n - 2; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            values[i] -= LD[j][i] * values[j];
        }
    }

    for (int i = 0; i < n; i++) {
        result[i] = values[i];
    }
}