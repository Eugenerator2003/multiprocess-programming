#pragma once
// контекст СЛАУ
class LinearSystemContext
{
private:
	double* values;
	double** matrix;
	double* x;
	
	int elements_count;
public:
	LinearSystemContext(int elements_count);

	~LinearSystemContext();

	void setValue(int k, double value);
	void setMatrixValue(int i, int j, int value);
	void setX(int k, double value);

	double* getValues();
	double** getMatrix();
	double* getX();
	int getElementsCount();

	// решение методом LD-разложения
	void solveLD();
};

