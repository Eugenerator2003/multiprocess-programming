#pragma once
#include <fstream>
#include <string>
#include "PointCollection.h"
#include "geometry.h"
const int MAX_COORD_VALUE = 1000;;
const int MIN_COORD_VALUE = -1000;;

// Вывод точки в поток
std::ostream& operator<<(std::ostream& os, const Point& p);

// Вывод треугольника в поток
std::ostream& operator<<(std::ostream& os, const Triangle& t);

// Чтение коллекции точек из файла
PointCollection* readPoints(std::string name);

// Запись коллекции точек в файл
void writePoints(std::string name, PointCollection* points);

// Запись треугольника в файл
void writeTriangles(std::string name, Triangle* triangle, int n);

// Генерация и запись точек
void generateWritePoints(std::string name, int n = 1000, int min = MIN_COORD_VALUE, int max = MAX_COORD_VALUE);