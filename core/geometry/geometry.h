#pragma once

struct Point
{
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	double x, y;
};

Point operator+ (const Point& pt1, const Point& pt2);
Point operator- (const Point& pt1, const Point& pt2);

using Vector = Point;
double operator* (const Vector& v1, const Vector& v2);
Vector operator* (double scalar, const Vector& v);
Vector operator* (const Vector& v, double scalar);