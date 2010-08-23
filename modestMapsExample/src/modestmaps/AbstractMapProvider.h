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

    double wrappedColumn = fmod(coordinate.column, gridSize);
    while (wrappedColumn < 0) {
      wrappedColumn += gridSize;
    }

    double wrappedRow = fmod(coordinate.row, gridSize);
    while (wrappedRow < 0) {
      wrappedRow += gridSize;
    }

    return Coordinate(wrappedRow, wrappedColumn, coordinate.zoom);
  }
  
};

#endif