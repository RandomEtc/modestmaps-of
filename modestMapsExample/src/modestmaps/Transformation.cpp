#include "Transformation.h"
#include "Point2d.h"

Point2d Transformation::transform(Point2d point) {
	return Point2d(ax*point.x + bx*point.y + cx, ay*point.x + by*point.y + cy);
}

Point2d Transformation::untransform(Point2d point) {
	return Point2d((point.x*by - point.y*bx - cx*by + cy*bx) / (ax*by - ay*bx), (point.x*ay - point.y*ax - cx*ay + cy*ax) / (bx*ay - by*ax));
}
