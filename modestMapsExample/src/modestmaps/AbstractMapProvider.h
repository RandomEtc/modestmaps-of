#ifndef ABSTRACT_PROVIDER
#define ABSTRACT_PROVIDER

#include "AbstractProjection.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractMapProvider {
	
public:
	
	AbstractProjection *projection;
	
	AbstractMapProvider(AbstractProjection *_projection): projection(_projection) {}
	
	virtual vector<string> getTileUrls(Coordinate coordinate)=0;
	virtual int tileWidth()=0;
	virtual int tileHeight()=0;
	
	Coordinate locationCoordinate(Location location) {
		return projection->locationCoordinate(location);
	}
	
	Location coordinateLocation(Coordinate coordinate) {
		return projection->coordinateLocation(coordinate);
	}
	
	Coordinate sourceCoordinate(Coordinate coordinate) {
		double gridSize = pow(2.0, coordinate.zoom);
		
		double wrappedColumn = coordinate.column;
		if(wrappedColumn >= gridSize) {
			wrappedColumn = fmod(wrappedColumn, gridSize);
		}
		else {
			while (wrappedColumn < 0) {
				wrappedColumn += gridSize;
			}
		}
		
		return Coordinate(coordinate.row, wrappedColumn, coordinate.zoom);
	}
	
};

#endif