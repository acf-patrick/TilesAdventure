#include "geometry.h"


Point operator+ (const Point& pt1, const Point& pt2)
{
	return { pt1.x + pt2.x, pt1.y + pt2.y };
}

Point operator- (const Point& pt1, const Point& pt2)
{
	return { pt1.x - pt2.x, pt1.y - pt2.y };
}

double operator* (const Vector& v1, const Vector& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

Vector operator* (double scalar, const Vector& v)
{
	return { scalar * v.x, scalar * v.y };
}

Vector operator* (const Vector& v, double scalar)
{
	return scalar * v;
}