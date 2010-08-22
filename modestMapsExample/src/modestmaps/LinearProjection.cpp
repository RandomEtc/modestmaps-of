#include "LinearProjection.h"

Point2d LinearProjection::rawProject(Point2d point) {
	return Point2d(point.x, point.y);
}

Point2d LinearProjection::rawUnproject(Point2d point) {
	return Point2d(point.x, point.y);
}
