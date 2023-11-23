#pragma once
#include "geometry.h"
class PointCollection
{
public:
	PointCollection(int n);
	~PointCollection();
	void setPoint(int i, Point point);
	Point* getPoint(int i);
	int size();
private:
	int n;
	Point* points;
};

