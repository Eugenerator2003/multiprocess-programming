#pragma once
#include <iostream>
#include <vector>

// Точка в пространстве
struct Point {
	double x;
	double y;
	double z;
};

// Треугольник
struct Triangle {
	Point* first;
	Point* second;
	Point* third;
	double square;
	double angles[3];
};

// Получение расстояния между точками в квадрате
double getSquareDistance(Point* first, Point* second);

double getDistance(Point* first, Point* second);

// Получение углов треугольника
void setTriangleValues(Triangle* triangle);

// Проверка на тупоугольность
bool isTriangleObtuse(const Triangle* triangle);