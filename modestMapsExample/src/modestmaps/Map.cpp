#include "Map.h"
#include "QueueSorter.h"

void Map::setup(AbstractMapProvider* _provider, double _width, double _height) {
	provider = _provider;
	width = _width;
	height = _height;
	tx = -TILE_SIZE/2.0; // half the world width, at zoom 0
	ty = -TILE_SIZE/2.0; // half the world height, at zoom 0
	// fit to screen
	sc = ceil(min(height/TILE_SIZE, width/TILE_SIZE));		
}
	
void Map::update() {
	// TODO: Move non-drawing logic here
}

void Map::draw() {
	
	// if we're in between zoom levels, we need to choose the nearest:
	int baseZoom = bestZoomForScale((float)sc);
	
	// these are the top left and bottom right tile coordinates
	// we'll be loading everything in between:
	Coordinate startCoord = pointCoordinate(Point2d(0,0)).zoomTo(baseZoom).container();
	Coordinate endCoord = pointCoordinate(Point2d(width,height)).zoomTo(baseZoom).container().right().down();
	
	// find start and end columns
	int minCol = startCoord.column;
	int maxCol = endCoord.column;
	int minRow = startCoord.row;
	int maxRow = endCoord.row;
	
	// pad a bit, for luck (well, because we might be zooming out between zoom levels)
	minCol -= GRID_PADDING;
	minRow -= GRID_PADDING;
	maxCol += GRID_PADDING;
	maxRow += GRID_PADDING;
	
	visibleKeys.clear();
	
	// grab coords for visible tiles
	for (int col = minCol; col <= maxCol; col++) {
		for (int row = minRow; row <= maxRow; row++) {
			
			// source coordinate wraps around the world:
			//Coordinate coord = provider->sourceCoordinate(Coordinate(row,col,baseZoom));
			Coordinate coord = Coordinate(row,col,baseZoom);
			
			// keep this for later:
			visibleKeys.insert(coord);
			
			if (images.count(coord) == 0) {
				
				// fetch it if we don't have it
				grabTile(coord);
				
				// see if we have  a parent coord for this tile?
				bool gotParent = false;
				for (int i = (int)coord.zoom; i > 0; i--) {
					Coordinate zoomed = coord.zoomTo(i).container();
					if (images.count(zoomed) > 0) {
						visibleKeys.insert(zoomed);
						gotParent = true;
						break;
					}
				}
				
				// or if we have any of the children
				if (!gotParent) {
					Coordinate zoomed = coord.zoomBy(1).container();
					vector<Coordinate> kids;
					kids.push_back(zoomed);
					kids.push_back(zoomed.right());
					kids.push_back(zoomed.down());
					kids.push_back(zoomed.right().down());
					for (int i = 0; i < kids.size(); i++) {
						if (images.count(kids[i]) > 0) {
							visibleKeys.insert(kids[i]);
						}
					}            
				}
				
			}
			
		} // rows
	} // columns
	
	// TODO: sort by zoom so we draw small zoom levels (big tiles) first:
	// can this be done with a different comparison function on the visibleKeys set?
	//Collections.sort(visibleKeys, zoomComparator);
	
	// translate and scale, from the middle
	ofPushMatrix();
	ofTranslate(width/2.0, height/2.0);
	ofScale(sc,sc,1);
	ofTranslate(tx, ty);
	
	int numDrawnImages = 0;
	
	if (visibleKeys.size() > 0) {
		double prevZoom = baseZoom;
		ofPushMatrix();
		// correct the scale for this zoom level:
		double correction = 1.0/pow(2.0, prevZoom);
		ofScale(correction,correction,1);
		set<Coordinate>::iterator iter;
		for (iter = visibleKeys.begin(); iter != visibleKeys.end(); iter++) {
			Coordinate coord = *iter;
			if (coord.zoom != prevZoom) {
				ofPopMatrix();
				ofPushMatrix();
				// correct the scale for this zoom level:
				correction = 1.0/pow(2.0,coord.zoom);
				ofScale(correction, correction, 1);
				prevZoom = coord.zoom;
			}

			//ofEnableAlphaBlending();
			//ofSetColor(0,0,0,50);
			//ofRect(coord.column*TILE_SIZE,coord.row*TILE_SIZE,TILE_SIZE,TILE_SIZE);
			
			if (images.count(coord) > 0) {
				//cout << "rendering: " << coord;				
				ofMemoryImage *tile = images[coord];
				// TODO: must be a cleaner C++ way to do this?
				// we want this image to be at the end of recentImages, if it's already there we'll remove it and then add it again
				vector<ofMemoryImage*>::iterator result = find(recentImages.begin(), recentImages.end(), tile);
				if (result != recentImages.end()) {
					recentImages.erase(result);
				}
				else {
					// if it's not in recent images it must be brand new?
					tile->setUseTexture(true);
					tile->initTex();
				}
				tile->draw(coord.column*TILE_SIZE,coord.row*TILE_SIZE,TILE_SIZE,TILE_SIZE);
				numDrawnImages++;
				recentImages.push_back(tile);
			}
		}
		ofPopMatrix();
	}    
	
	ofPopMatrix();
		
	// stop fetching things we can't see:
	// (visibleKeys also has the parents and children, if needed, but that shouldn't matter)
	//queue.retainAll(visibleKeys);
	vector<Coordinate>::iterator iter = queue.begin();
	while (iter != queue.end()) {
		Coordinate key = *iter;
		if (visibleKeys.count(key) == 0){ 
			iter = queue.erase(iter);
		}
		else {
			++iter;
		}
	}
	
	// TODO sort what's left by distance from center:
	//queueSorter.setCenter(new Coordinate( (minRow + maxRow) / 2.0f, (minCol + maxCol) / 2.0f, zoom));
	//Collections.sort(queue, queueSorter);
	
	// load up to 4 more things:
	processQueue();
	
	// clear some images away if we have too many...
	int numToKeep = max(numDrawnImages,MAX_IMAGES_TO_KEEP);
	if (recentImages.size() > numToKeep) {
		// first clear the pointers from recentImages
		recentImages.erase(recentImages.begin(), recentImages.end()-numToKeep);
		//images.values().retainAll(recentImages);
		// TODO: re-think the stl collections used so that a simpler retainAll equivalent is available
		// now look in the images map and if the value is no longer in recent images then get rid of it
		map<Coordinate,ofMemoryImage*>::iterator iter = images.begin();
		map<Coordinate,ofMemoryImage*>::iterator endIter = images.end();
		for (; iter != endIter;) {
			ofMemoryImage* tile = iter->second;
			vector<ofMemoryImage*>::iterator result = find(recentImages.begin(), recentImages.end(), tile);
			if (result == recentImages.end()) {
				images.erase(iter++);
				delete tile;
			}
			else {
				++iter;
			}
		}
	}
	
}

void Map::keyPressed(int key) {
	if (key == '+' || key == '=') {
		if (getZoom() < 19) {
			zoomIn();
		}
	}
	else if (key == '-' || key == '_') {
		if (getZoom() > 0) {
			zoomOut();
		}
	}
	// TODO: keyboard movement
}
void Map::keyReleased(int key) {
	// TODO: keyboard movement
}
void Map::mouseDragged(int x, int y, int button) {
	double dx = ((double)x - px) / sc;
	double dy = ((double)y - py) / sc;
	if (button == GLUT_LEFT_BUTTON) {
		//cout << dx << "," << dy;
		//    float angle = radians(-a);
		//    float rx = cos(angle)*dx - sin(angle)*dy;
		//    float ry = sin(angle)*dx + cos(angle)*dy;
		//    tx += rx;
		//    ty += ry;
		tx += dx;
		ty += dy;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		double mx = x - width/2;
		double my = x - height/2;
		tx -= mx/sc;
		ty -= my/sc;
		if (dy < 0) {
			sc *= 1.0 + (fabs(dy*sc) / 100.0);
		}
		else {
			sc /= 1.0 + ((dy*sc) / 100.0);
		}
		tx += mx/sc;
		ty += my/sc;		
	}
	px = x;
	py = y;
}
void Map::mousePressed(int x, int y, int button) {
	// TODO: doubleclick?
	px = x;
	py = y;
}
void Map::mouseReleased(int x, int y, int button) {
	// TODO: doubleclick?
}

//////////////////


/** @return zoom level of currently visible tile layer */
int Map::getZoom() {
	return bestZoomForScale((float)sc);
}

Location Map::getCenter() {
	return provider->coordinateLocation(getCenterCoordinate());
}

Coordinate Map::getCenterCoordinate() {
	float row = (float)(ty*sc/-TILE_SIZE);
	float column = (float)(tx*sc/-TILE_SIZE);
	float zoom = zoomForScale((float)sc);
	return Coordinate(row, column, zoom); 
}

void Map::setCenter(Coordinate center) {
	//println("setting center to " + center);
	sc = pow(2.0, center.zoom);
	tx = -TILE_SIZE*center.column/sc;
	ty = -TILE_SIZE*center.row/sc;
}

void Map::setCenter(Location location) {
	setCenter(provider->locationCoordinate(location).zoomTo(getZoom()));
}

void Map::setCenterZoom(Location location, int zoom) {
	setCenter(provider->locationCoordinate(location).zoomTo(zoom));
}

/** sets scale according to given zoom level, should leave you with pixel perfect tiles */
void Map::setZoom(int zoom) {
	sc = pow(2.0, zoom); 
}

void Map::zoom(int dir) {
	sc = pow(2.0, getZoom()+dir); 
}

void Map::zoomIn() {
	sc = pow(2.0, getZoom()+1); 
}  

void Map::zoomOut() {
	sc = pow(2.0f, getZoom()-1); 
}

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

Point2d Map::coordinatePoint(Coordinate coord)
{
	/* Return an x, y point on the map image for a given coordinate. */
	
	Coordinate center = getCenterCoordinate();
	
	if(coord.zoom != center.zoom) {
		coord = coord.zoomTo(center.zoom);
	}
	
	// distance from the center of the map
	Point2d point = Point2d(width/2, height/2);
	point.x += TILE_SIZE * (coord.column - center.column);
	point.y += TILE_SIZE * (coord.row - center.row);
	
	return point;
}

Coordinate Map::pointCoordinate(Point2d point) {
	/* Return a coordinate on the map image for a given x, y point. */		
	// new point coordinate reflecting distance from map center, in tile widths
	Coordinate coord = getCenterCoordinate();
	coord.column += (point.x - width/2) / TILE_SIZE;
	coord.row += (point.y - height/2) / TILE_SIZE;
	return coord;
}

Point2d Map::locationPoint(Location location) {
	return coordinatePoint(provider->locationCoordinate(location));
}

Location Map::pointLocation(Point2d point) {
	return provider->coordinateLocation(pointCoordinate(point));
}

// TODO: pan by proportion of screen size, not by coordinate grid
void Map::panUp() {
	setCenter(getCenterCoordinate().up());
}
void Map::panDown() {
	setCenter(getCenterCoordinate().down());
}
void Map::panLeft() {
	setCenter(getCenterCoordinate().left());
}
void Map::panRight() {
	setCenter(getCenterCoordinate().right());
}

void Map::panAndZoomIn(Location location) {
	// TODO: animate?
	setCenterZoom(location, getZoom() + 1);
}

void Map::panTo(Location location) {
	// TODO: animate?
	setCenter(location);
}

///////////////////////////////////////////////////////////////////////

float Map::scaleForZoom(int zoom) {
	return pow(2.0, zoom);
}

float Map::zoomForScale(float scale) {
	// TODO log(2) const?
	return log(scale) / log(2);
}

int Map::bestZoomForScale(float scale) {
	// TODO get min/max zoom level from provider?
	// TODO log(2) const?
	return (int)min(20, max(1, (int)round(log(scale) / log(2))));
}

//////////////////////////////////////////////////////////////////////////

void Map::grabTile(Coordinate coord) {
	bool isPending = pending.count(coord) > 0;
	bool isQueued = find(queue.begin(), queue.end(), coord) != queue.end();
	bool isAlreadyLoaded = images.count(coord) > 0;
	if (!isPending && !isQueued && !isAlreadyLoaded) {
		queue.push_back(Coordinate(coord));
	}
}


// TODO: there could be issues when this is called from within a thread
// probably needs synchronizing on images / pending / queue
void Map::tileDone(Coordinate coord, ofMemoryImage *img) {
	// check if we're still waiting for this (new provider clears pending)
	// also check if we got something
	if (pending.count(coord) > 0 && img != NULL) {
		images[Coordinate(coord)] = img;
		pending.erase(coord);  
	}
	else {
		pending.erase(coord);
		// try again?
	}
}

void Map::processQueue() {
	if (queue.size() > MAX_PENDING-pending.size()) {
		sort(queue.begin(), queue.end(), QueueSorter(getCenterCoordinate().zoomTo(getZoom())));		
	}
	while (pending.size() < MAX_PENDING && queue.size() > 0) {
		Coordinate coord = *(queue.begin());
		Coordinate key = Coordinate(coord);
		pending[key] = TileLoader();
		pending[key].start(key, provider, this);
		queue.erase(queue.begin());
	}  
}

