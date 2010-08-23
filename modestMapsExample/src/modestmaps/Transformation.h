#ifndef TRANSFORMATION
#define TRANSFORMATION

class Point2d;

class Transformation {
	
public:
	
	double ax, bx, cx, ay, by, cy;

	Transformation() : ax(1), bx(0), cx(0), ay(0), by(1), cy(0) {}
	
	Transformation(float _ax, float _bx, float _cx, float _ay, float _by, float _cy) : ax(_ax), bx(_bx), cx(_cx), ay(_ay), by(_by), cy(_cy) {}
	
	Transformation(const Transformation &t) : ax(t.ax), bx(t.bx), cx(t.cx), ay(t.ay), by(t.by), cy(t.cy) {}
	
	Point2d transform(Point2d point);
	
	Point2d untransform(Point2d point);
	
};

#endif