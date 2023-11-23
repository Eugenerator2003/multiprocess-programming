#include <sstream>
#include <random>
#include "geometryio.h"

std::ostream& operator<<(std::ostream& os, const Point& p) {
	return os << p.x << " " << p.y << " " << p.z;
}

std::ostream& operator<<(std::ostream& os, const Triangle& t)
{
	os << *t.first << " | ";
	os << *t.second << " | ";
	os << *t.third << " | square: ";
	os << t.square << " | ";
	os << t.angles[0] << ", ";
	os << t.angles[1] << ", ";
	os << t.angles[2];
	return os;
}

PointCollection* readPoints(std::string name)
{
	PointCollection* points = nullptr;
	std::ifstream in(name);
	if (in.is_open()) {
		std::string line;
		int n;
		std::getline(in, line);
		n = std::stod(line);
		points = new PointCollection(n);
		int i = 0;
		std::string word;
		while (i < n) {
			std::getline(in, line);
			std::stringstream ss(line);
			ss >> word;
			double x = std::stod(word);
			ss >> word;
			double y = std::stod(word);
			ss >> word;
			double z = std::stod(word);
			points->setPoint(i, Point{ x, y, z });
			i++;
		}
		in.close();
	}
	return points;
}

void writePoints(std::string name, PointCollection* points)
{
	std::ofstream out(name);
	if (out.is_open()) {
		int n = points->size();
		out << n << std::endl;
		for (int i = 0; i < n; i++) {
			out << *points->getPoint(i) << std::endl;
		}
		out.close();
	}
}

void writeTriangles(std::string name, Triangle* triangle, int n)
{
	std::ofstream out(name);
	if (out.is_open()) {
		out << n << std::endl;
		for (int i = 0; i < n; i++) {
			double* angles = triangle[i].angles;
			out << triangle[i] << std::endl;
		}
	}
	out.close();
}

double getRandom(std::mt19937& rand, int min, int max) {
	return (double)min + (rand() % (max - min));
}

void generateWritePoints(std::string name, int n, int min, int max) {
	PointCollection pts = PointCollection(n);
	std::random_device rd;
	std::mt19937 mersenne(rd());
	double x, y, z;
	for (int i = 0; i < n; i++) {
		x = getRandom(mersenne, min, max);
		y = getRandom(mersenne, min, max);
		z = getRandom(mersenne, min, max);
		pts.setPoint(i, Point{ x, y, z });
	}

	writePoints(name, &pts);
}
