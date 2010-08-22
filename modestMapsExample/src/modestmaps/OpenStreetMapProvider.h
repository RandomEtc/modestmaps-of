#ifndef OPENSTREETMAPPROVIDER
#define OPENSTREETMAPPROVIDER

#include "ofMain.h"
#include "AbstractMapProvider.h"
#include "MercatorProjection.h"

class OpenStreetMapProvider : public AbstractMapProvider {
	
public:
	
	//public String[] subdomains = new String[] { "", "a.", "b.", "c." };
	
	OpenStreetMapProvider(): 
		// this is the projection and transform you'll want for any Google-style map tile source:
		AbstractMapProvider(new MercatorProjection(26, Transformation(1.068070779e7, 0.0, 3.355443185e7, 0.0, -1.068070890e7, 3.355443057e7)))
	{}
	
	int tileWidth() {
		return 256;
	}
	
	int tileHeight() {
		return 256;
	}
	
	vector<string> getTileUrls(Coordinate coordinate) {
		stringstream url;
		// TODO: Use a random subdomain
		//url << "http://tile.openstreetmap.org/" << (int)coordinate.zoom << "/" << (int)coordinate.column << "/" << (int)coordinate.row << ".png";
		url << "/Users/tom/Documents/Stamen/SFMOMA/sfmoma/trunk/processing/world_borders_render/gray-world/" << (int)coordinate.zoom << "/" << (int)coordinate.column << "/" << (int)coordinate.row << ".png";
		vector<string> urls;
		urls.push_back(url.str());
		return urls;
	}
	
};

#endif