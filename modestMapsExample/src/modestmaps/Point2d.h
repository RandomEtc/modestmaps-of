#ifndef POINT_2D
#define POINT_2D

#include "ofMain.h"

class Point2d {
		
public:
	double x;
	double y;	

	Point2d() : x(0), y(0) {}

	Point2d(double _x, double _y) : x(_x), y(_y) {}

	Point2d(const Point2d &point) : x(point.x), y(point.y) {}
	
	friend ostream &operator<<(ostream &stream, Point2d p) {
		// TODO: number format
		stream << "(" << p.x << ", " << p.y << ")";
		return stream;
	}
	
    bool operator == (const Point2d &p) const
    {
        return p.x == x && p.y == y;
    }
	
};

#endif

