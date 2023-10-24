#include "LinearSystemContext.h"
#include "matrix_solving.h"
#include <cstring>

LinearSystemContext::LinearSystemContext(int elements_count) {
	values = new double[elements_count];
	matrix = new double*[elements_count];
	for (int i = 0; i < elements_count; i++) {
		matrix[i] = new double[elements_count];
	}
	x = new double[elements_count];
	this->elements_count = elements_count;
}

LinearSystemContext::~LinearSystemContext() {
	delete[] values;
	for (int i = 0; i < elements_count; i++) {
		delete[] matrix[i];
		matrix[i] = NULL;
	}
	delete[] matrix;
	delete[] x;
}

void LinearSystemContext::setValue(int k, double value)
{
	values[k] = value;
}

void LinearSystemContext::setMatrixValue(int i, int j, int value)
{
	matrix[i][j] = value;
}

void LinearSystemContext::setX(int k, double value)
{
	x[k] = value;
}

double* LinearSystemContext::getValues()
{
	return values;
}

double** LinearSystemContext::getMatrix()
{
	return matrix;
}

double* LinearSystemContext::getX()
{
	return x;
}

int LinearSystemContext::getElementsCount()
{
	return this->elements_count;
}

void LinearSystemContext::solveLD()
{
    int n = elements_count;
    double* members = new double[elements_count];
    double** LD = new double* [elements_count];
    for (int i = 0; i < elements_count; i++) {
        members[i] = values[i];
        LD[i] = new double[elements_count];
        for (int j = 0; j < elements_count; j++) {
            LD[i][j] = matrix[i][j];
        }
    }

    ld_decomposition(LD, n);
    ld_solve(LD, members, elements_count, x);

    for (int i = 0; i < n; i++) {
        delete[] LD[i];
        LD[i] = NULL;
    }

    delete[] LD;
    delete[] members;
}