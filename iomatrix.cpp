#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <cstdlib>
#include <filesystem>
#include "iomatrix.h"

void print(LinearSystemContext& context) {
	int n = context.getElementsCount();
	double** matr = context.getMatrix();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << matr[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------------" << std::endl;
}

// чтение массива свободных членов из текстового файла в контекст СЛАУ
void readarr(std::string name, LinearSystemContext* context)
{
	std::ifstream in(name);
	if (in.is_open()) {
		std::string line;
		std::getline(in, line);
		int n = std::stod(line);

		std::getline(in, line);
		std::string word;
		std::stringstream ss(line);
		int i = 0;
		while (ss >> word)
		{
			context->setValue(i, std::stod(word));
			i++;
		}
	}
	in.close();
}

// чтение матрицы из файла с созданием контекста СЛАУ
LinearSystemContext* readmatrix(std::string name)
{
	LinearSystemContext* context = nullptr;
	std::ifstream in(name);
	if (in.is_open()) {
		std::string line = "";
		std::getline(in, line);
		int n = std::stod(line);
		context = new LinearSystemContext(n);

		for (int i = 0; i < n; i++) {
			std::getline(in, line);

			std::string word;
			std::stringstream ss(line);
			int j = 0;
			while (ss >> word) {
				context->setMatrixValue(i, j, std::stod(word));
				j++;
			}
		}
		in.close();
	}
	return context;
}

// запись матрицы в текстовый файл
void writematrix(std::string name, LinearSystemContext* context)
{
	int n = context->getElementsCount();
	std::ofstream out(name, std::ios::out);
	out << n << " " << n << std::endl;
	double** matr = context->getMatrix();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			out << matr[i][j] << " ";
		}
		out << std::endl;
	}
	out.close();
}

// запись свободных членов или решения СЛАУ в текстовый файл
void writearr(std::string name, LinearSystemContext* context, bool result) {
	int n = context->getElementsCount();
	double* arr;
	if (result) {
		arr = context->getX();
	}
	else
	{
		arr = context->getValues();
	}
	std::ofstream out(name, std::ios::out);
	out << n << std::endl;
	for (int i = 0; i < n; i++) {
		out << arr[i] << " ";
	}
	out.close();
}

// генерация свободных членов СЛАУ
void genarray(LinearSystemContext* context, int maxvalue, std::mt19937* mersenne)
{
	bool created = false;
	if (!mersenne) {
		std::random_device seed;
		mersenne = new std::mt19937(seed());
		created = true;
	}

	int n = context->getElementsCount();

	for (int i = 0; i < n; i++) {
		context->setValue(i, (*mersenne)() % (1 + maxvalue));
	}

	if (created) {
		delete mersenne;
	}
}

// генерация матрицы с созданием контекста СЛАУ
LinearSystemContext genmatrix(int maxvalue, std::mt19937* mersenne)
{
	bool created = false;
	if (!mersenne) {
		std::random_device seed;
		mersenne = new std::mt19937(seed());
		created = true;
	}

	int n = 100 + (*mersenne)() % (50 + 200);

	LinearSystemContext context = LinearSystemContext(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			context.setMatrixValue(i, j, (*mersenne)() % (1 + maxvalue));
		}
	}

	if (created) {
		delete mersenne;
	}

	return context;
}

// генерация СЛАУ и их запись в файл
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
		LinearSystemContext context = genmatrix(MAX_VAL);
		genarray(&context, MAX_VAL);
		writematrix("matrices/" + std::to_string(i) + ".txt", &context);
		writearr("values/" + std::to_string(i) + ".txt", &context);
	}
}
