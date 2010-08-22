#include "MercatorProjection.h"
#include "Point2d.h"

Point2d MercatorProjection::rawProject(Point2d point) {
	return Point2d(point.x, log(tan(0.25 * PI + 0.5 * point.y)));
}

Point2d MercatorProjection::rawUnproject(Point2d point) {
	return Point2d(point.x, 2.0 * atan(pow(M_E, point.y)) - 0.5 * PI);
}
