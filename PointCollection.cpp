#include "PointCollection.h"

PointCollection::PointCollection(int n)
{
	this->n = n;
	points = new Point[n];
}

PointCollection::~PointCollection()
{
	delete[] points;
}

void PointCollection::setPoint(int i, Point point)
{
	points[i] = point;
}

Point* PointCollection::getPoint(int i)
{
	return &points[i];
}

int PointCollection::size()
{
	return n;
}
