#ifndef ABSTRACT_PROJECTION
#define ABSTRACT_PROJECTION

#include "Transformation.h"
#include "Point2d.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractProjection {

public:

	double zoom;
	Transformation transformation;

	AbstractProjection(double _zoom): zoom(_zoom), transformation(Transformation()) { }
	AbstractProjection(double _zoom, Transformation _t): zoom(_zoom), transformation(_t) { }

	virtual Point2d rawProject(Point2d point)=0;
	virtual Point2d rawUnproject(Point2d point)=0;

	Point2d project(Point2d point) {
		return transformation.transform(rawProject(point));
	}

	Point2d unproject(Point2d point) {
		return rawUnproject(transformation.untransform(point));
	}

	Coordinate locationCoordinate(Location location) {
		Point2d point = project(Point2d(DEG_TO_RAD * location.lon, DEG_TO_RAD * location.lat));
		return Coordinate(point.y, point.x, zoom);
	}

	Location coordinateLocation(Coordinate coordinate) {
		coordinate = coordinate.zoomTo(zoom);
		Point2d point = unproject(Point2d(coordinate.column, coordinate.row));
		return Location(RAD_TO_DEG * point.y, RAD_TO_DEG * point.x);
	}

};

#endif