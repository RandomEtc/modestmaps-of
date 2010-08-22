#ifndef TILE_LOADER
#define TILE_LOADER

#include "ofMain.h"
#include "ofxThread.h"
#include "Coordinate.h"

class AbstractMapProvider;
class Map;

class TileLoader : public ofxThread {
	
public:
	
    Coordinate coord;
	AbstractMapProvider *provider;
	Map *theMap;
	
	void start(Coordinate _coord, AbstractMapProvider *_provider, Map *_map) {
		coord = _coord;
		provider = _provider;
		theMap = _map;
		startThread(true,false);
	}
	
    void threadedFunction();
	
};

	
#endif