#pragma once
#include <string>
#include <random>
#include "LinearSystemContext.h"
constexpr auto MAX_VAL = 200;;

// ����� �������
void print(LinearSystemContext &context);

// ������ ������� �� �����
void readarr(std::string name, LinearSystemContext* context);

// ������ ������� �� �����
LinearSystemContext* readmatrix(std::string name);

// ������ ������� � ����
void writematrix(std::string name, LinearSystemContext* context);

// ������ ������� � ����
void writearr(std::string name, LinearSystemContext* context, bool result = false);

// ��������� �������
void genarray(LinearSystemContext* context, int maxvalue, std::mt19937* mersenne = nullptr);

// ��������� �������
LinearSystemContext genmatrix(int maxvalue, std::mt19937* mersenne = nullptr);

// ��������� ������ � �������� � ����������� ������� � ����
void genwritematrices(int count);