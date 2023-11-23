#pragma once
#include <iostream>
#include <vector>

// ����� � ������������
struct Point {
	double x;
	double y;
	double z;
};

// �����������
struct Triangle {
	Point* first;
	Point* second;
	Point* third;
	double square;
	double angles[3];
};

// ��������� ���������� ����� ������� � ��������
double getSquareDistance(Point* first, Point* second);

double getDistance(Point* first, Point* second);

// ��������� ����� ������������
void setTriangleValues(Triangle* triangle);

// �������� �� ��������������
bool isTriangleObtuse(const Triangle* triangle);