#pragma once
#include <fstream>
#include <string>
#include "PointCollection.h"
#include "geometry.h"
const int MAX_COORD_VALUE = 1000;;
const int MIN_COORD_VALUE = -1000;;

// ����� ����� � �����
std::ostream& operator<<(std::ostream& os, const Point& p);

// ����� ������������ � �����
std::ostream& operator<<(std::ostream& os, const Triangle& t);

// ������ ��������� ����� �� �����
PointCollection* readPoints(std::string name);

// ������ ��������� ����� � ����
void writePoints(std::string name, PointCollection* points);

// ������ ������������ � ����
void writeTriangles(std::string name, Triangle* triangle, int n);

// ��������� � ������ �����
void generateWritePoints(std::string name, int n = 1000, int min = MIN_COORD_VALUE, int max = MAX_COORD_VALUE);