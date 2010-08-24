
#ifndef MODEST_MAP
#define MODEST_MAP

#include <set>
#include <map>
#include "ofMain.h"
#include "AbstractMapProvider.h"
#include "Location.h"
#include "Coordinate.h"
#include "Point2d.h"
#include "TileLoader.h"
#include "ofMemoryImage.h"

#define TILE_SIZE 256.0

// limit simultaneous calls to loadImage
#define MAX_PENDING  8

// limit tiles in memory
// 256 would be 64 MB, you may want to lower this quite a bit for your app
// (we'll always keep as many images as needed to fill the screen though)
#define MAX_IMAGES_TO_KEEP 256

// upping this can help appearances when zooming out, but also loads many more tiles
#define GRID_PADDING 1


class Map {
	
public:
	
	// pan and zoom
	double tx, ty, sc;
	
	// what kinda maps?
	AbstractMapProvider* provider;
	
	// how big?
	double width, height;
	
	// loading tiles
	map<Coordinate, TileLoader> pending;
	// loaded tiles
	map<Coordinate, ofMemoryImage*> images;
	// coords waiting to load
	vector<Coordinate> queue;
	// a list of the most recent MAX_IMAGES_TO_KEEP ofImages we've seen
	vector<ofMemoryImage*> recentImages;
	// keep track of what we can see already:
	set<Coordinate> visibleKeys;
	
	// for sorting coordinates by zoom
	//ZoomComparator zoomComparator;
	
	// for loading tiles from the inside first
	//QueueSorter queueSorter;

	double px;
	double py;
	
	/////////////////////////// methods	
	
	Map() {}
	
	void setup(AbstractMapProvider* _provider, double _width, double _height);	
	void update();
	void draw();
		
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	int getZoom();	
	Location getCenter();
	Coordinate getCenterCoordinate();
	
	void setCenter(Coordinate center);
	
	void setCenter(Location location);
	
	void setCenterZoom(Location location, int zoom);
	
	void setZoom(int zoom);
	
	void zoom(int dir);
	
	void zoomIn();
	
	void zoomOut();
	
	// TODO: extent functions
	//	    public function setExtent(extent:MapExtent):void
	//	    public function getExtent():MapExtent
	
	// TODO: make it so you can safely get and set the provider
	/*AbstractMapProvider getMapProvider() {
	    return provider;
	}
	
	void setMapProvider(AbstractMapProvider _provider) {
		provider = _provider;
		images.clear();
		queue.clear();
		pending.clear();
	}*/
	
	Point2d coordinatePoint(Coordinate coord);
    
	Coordinate pointCoordinate(Point2d point);
	
	Point2d locationPoint(Location location);
	
	Location pointLocation(Point2d point);
	
	// TODO: pan by proportion of screen size, not by coordinate grid
	void panUp();
	void panDown();
	void panLeft();
	void panRight();
	
	void panAndZoomIn(Location location);
	
	void panTo(Location location);
	
	///////////////////////////////////////////////////////////////////////
	
	float scaleForZoom(int zoom);
	
	float zoomForScale(float scale);
	
	int bestZoomForScale(float scale);
	
	//////////////////////////////////////////////////////////////////////////
	
	
	void grabTile(Coordinate coord);
	
	void tileDone(Coordinate coord, ofMemoryImage *img);
	
	void processQueue();
	
};

#endif
