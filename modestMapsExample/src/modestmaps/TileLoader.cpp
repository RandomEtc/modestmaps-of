#include "AbstractMapProvider.h"
#include "TileLoader.h"
#include "Map.h"

void TileLoader::threadedFunction() {
	vector<string> urls = provider->getTileUrls(coord);
	ofImage* img = new ofImage();
	img->loadImage(urls[0]);
	/*if (img != null) {
	 for (int i = 1; i < urls.length; i++) {
	 PImage img2 = p.loadImage(urls[i], "unknown");
	 if (img2 != null) {
	 img.blend(img2, 0, 0, img.width, img.height, 0, 0, img.width, img.height, BLEND);
	 }
	 }
	 }*/
	theMap->tileDone(coord, img);
}
