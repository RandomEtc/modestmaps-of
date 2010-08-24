#ifndef COORDINATE
#define COORDINATE

class Coordinate {
	
public:
	
	double row;	
	double column;
	double zoom;
	
	Coordinate() : row(0), column(0), zoom(0) { }
	
	Coordinate(double _row, double _column, double _zoom) : row(_row), column(_column), zoom(_zoom) {}
	
	Coordinate(const Coordinate &coord) : row(coord.row), column(coord.column), zoom(coord.zoom) {}
	
	friend ostream &operator<<(ostream &stream, Coordinate c) {
		// TODO: number format
		stream << "(" << c.row << ", " << c.column << " @" << c.zoom << ")";
		return stream;
	}
	
    bool operator == (const Coordinate &c) const
    {
        return c.row == row && c.column == column && c.zoom == zoom;
    }

	bool operator < (const Coordinate &c) const
    {
		//cout << *this << " compared to " << c << endl;
        return zoom < c.zoom || (zoom == c.zoom && row < c.row) || (zoom == c.zoom && row == c.row && column < c.column);
    }
	
	Coordinate container() {
		return Coordinate(floor(row), floor(column), zoom);
	}
	
	Coordinate zoomTo(double destination) {
		return Coordinate(row * pow(2, destination - zoom),
						  column * pow(2, destination - zoom),
						  destination);
	}
	
	Coordinate zoomBy(double distance) {
		return Coordinate(row * pow(2, distance),
						  column * pow(2, distance),
						  zoom + distance);
	}
	
	Coordinate up(double distance=1) {
		return Coordinate(row - distance, column, zoom);
	}
	
	Coordinate right(double distance=1) {
		return Coordinate(row, column + distance, zoom);
	}
	
	Coordinate down(double distance=1) {
		return Coordinate(row + distance, column, zoom);
	}
	
	Coordinate left(double distance=1) {
		return Coordinate(row, column - distance, zoom);
	}
	
};

#endif
