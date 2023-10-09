#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <filesystem>
#include "iomatrix.h"

double* zeros(int n) {
	double* arr = new double[n];
	for (int i = 0; i < n; i++) {
		arr[i] = 0;
	}
	return arr;
}

double** zeros(int n, int m)
{
	double** matrx = new double*[n];
	for (int i = 0; i < n; i++) {
		matrx[i] = zeros(m);
	}
	return matrx;
}

double** copy(double** src, int n, int m)
{
	double** dst = new double*[n];
	for (int i = 0; i < n; i++) {
		dst[i] = new double[m];
		for(int j = 0; j < m; j++)
		{
			dst[i][j] = src[i][j];
		}
	}
	return dst;
}


void print(double** matrx, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cout << matrx[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------------" << std::endl;
}

double* readarr(std::string name, int* n)
{
	double* arr = nullptr;
	std::ifstream in(name);
	if (in.is_open()) {
		std::string line;
		std::getline(in, line);
		*n = atoi(line.c_str());

		arr = new double[*n];
		std::getline(in, line);

		char* ptrline = const_cast<char*>(line.c_str());
		char* next_token;
		char* pos = strtok_s(ptrline, " ", &next_token);
		arr[0] = atof(pos);
		
		int i = 1;
		while (pos = strtok_s(NULL, " ", &next_token))
		{
			arr[i] = atof(pos);
			i++;
		}
	}
	return arr;
}

double** readmatrix(std::string name, int* n, int *m)
{
	double** matr = nullptr;
	std::ifstream in(name);
	if (in.is_open()) {
		std::string line;
		std::getline(in, line);
		*n = atoi(const_cast<char*>(line.c_str()));
		matr = new double* [*n];
		for (int i = 0; i < *n; i++) {
			std::getline(in, line);

			char* ptrline = const_cast<char*>(line.c_str());
			char* next_token;
			char* pos = strtok_s(ptrline, " ", &next_token);

			matr[i] = new double[*n];
			matr[i][0] = atof(pos);

			int j = 1;
			while (pos = strtok_s(NULL, " ", &next_token)) {
				matr[i][j] = atof(pos);
				j++;
			}
		}
	}
	in.close();
	return matr;
}

void writematrix(std::string name, double** matr, int n, int m)
{
	std::ofstream out(name, std::ios::out);
	out << n << " " << m << std::endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			out << matr[i][j] << " ";
		}
		out << std::endl;
	}
	out.close();
}

void writearr(std::string name, double* arr, int n) {
	std::ofstream out(name, std::ios::out);
	out << n << std::endl;
	for (int i = 0; i < n; i++) {
		out << arr[i] << " ";
	}
	out.close();
}

double* genarray(int n, int maxvalue, std::mt19937* mersenne)
{
	bool created = false;
	if (!mersenne) {
		std::random_device seed;
		mersenne = new std::mt19937(seed());
		created = true;
	}

	if (n == NULL) {
		n = (*mersenne)() % (1 + 200);
	}

	double* arr = new double[n];
	for (int i = 0; i < n; i++) {
		arr[i] = (*mersenne)() % (1 + maxvalue);
	}

	if (created) {
		delete mersenne;
	}

	return arr;
}

double** genmatrix(int &n, int &m, int maxvalue, std::mt19937* mersenne)
{
	bool created = false;
	if (!mersenne) {
		std::random_device seed;
		mersenne = new std::mt19937(seed());
		created = true;
	}

	if (n == NULL && n == m) {
		n = 100 * (*mersenne)() % (1 + 200);
		m = n;
	}

	double** matr = new double*[n];
	for (int i = 0; i < n; i++) {
		matr[i] = genarray(m, MAX_VAL, mersenne);
	}

	if (created) {
		delete mersenne;
	}

	return matr;
}

void genwritematrices(int count)
{
	if (!std::filesystem::exists("matrices")) {
		std::filesystem::create_directory("matrices");
	}

	if (!std::filesystem::exists("values")) {
		std::filesystem::create_directory("values");
	}

	int n = NULL, m = NULL;
	for (int i = 0; i < count; i++) {
		double** matr = genmatrix(n, m, MAX_VAL);
		double* b = genarray(m);
		writematrix("matrices/" + std::to_string(i) + ".txt", matr, n, m);
		writearr("values/" + std::to_string(i) + ".txt", b, m);
		delete matr, b;
		n = NULL;
		m = NULL;
	}
}
