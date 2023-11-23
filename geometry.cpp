#include "geometry.h"

double getSquareDistance(Point* first, Point* second) {
	double x = first->x - second->x;
	double y = first->y - second->y;
	double z = first->z - second->z;
	return x * x + y * y + z * z;
}

double getDistance(Point* first, Point* second)
{
	return sqrt(getSquareDistance(first, second));
}


void setTriangleValues(Triangle* triangle) {

	Point* first = triangle->first;
	Point* second = triangle->second;
	Point* third = triangle->third;
	
	double a2 = getSquareDistance(first, second);
	double b2 = getSquareDistance(first, third);
	double c2 = getSquareDistance(second, third);
	double a = sqrt(a2);
	double b = sqrt(b2);
	double c = sqrt(c2);

	double* values[3][5] = {
		{ &c2, &a2, &b2, &a, &b},
		{ &a2, &c2, &b2, &c, &b},
		{ &b2, &a2, &c2, &a, &c}
	};

	for (int i = 0; i < 3; i++) {
		triangle->angles[i] = acos((*values[i][0] - *values[i][1] - *values[i][2]) / (-2 * *values[i][3] * *values[i][4]));
	}

	double half_p = (a + b + c) / 2;
	triangle->square = sqrt(half_p * (half_p - a) * (half_p - b) * (half_p - c));
}

bool isTriangleObtuse(const Triangle* triangle) {
	Point* first = triangle->first;
	Point* second = triangle->second;
	Point* third = triangle->third;
	double a, b, c;
	a = getSquareDistance(first, second);
	b = getSquareDistance(second, third);
	c = getSquareDistance(first, third);
	double max, min1, min2;
	if (a > b && a > c) {
		max = a;
		min1 = b;
		min2 = c;
	}
	else if (b > c && b > a) {
		max = b;
		min1 = c;
		min2 = a;
	}
	else {
		max = c;
		min1 = a;
		min2 = b;
	}

	return max > min1 + min2;
}