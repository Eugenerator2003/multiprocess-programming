#pragma once
#include <string>
#include <random>
#include "LinearSystemContext.h"
constexpr auto MAX_VAL = 200;;

// вывод матрицы
void print(LinearSystemContext &context);

// чтение массива из файла
void readarr(std::string name, LinearSystemContext* context);

// чтение матрицы из файла
LinearSystemContext* readmatrix(std::string name);

// запись матрицы в файл
void writematrix(std::string name, LinearSystemContext* context);

// запись массива в файл
void writearr(std::string name, LinearSystemContext* context, bool result = false);

// генерация массива
void genarray(LinearSystemContext* context, int maxvalue, std::mt19937* mersenne = nullptr);

// генерация матрицы
LinearSystemContext genmatrix(int maxvalue, std::mt19937* mersenne = nullptr);

// генерация матриц и массивов с последующей записью в файл
void genwritematrices(int count);