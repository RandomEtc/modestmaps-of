#include "testApp.h"
#include "OpenStreetMapProvider.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	//ofSetFrameRate(100);
	map.setup(new OpenStreetMapProvider(), (double)ofGetWidth(), (double)ofGetHeight());
	map.setZoom(3);
}

//--------------------------------------------------------------
void testApp::update(){
	map.update();
	//map.sc += 0.01 * sin(ofGetElapsedTimef());
	//map.tx += 0.01 * cos(ofGetElapsedTimef());
	//map.ty += 0.01 * sin(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void testApp::draw(){
	map.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	map.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	map.keyReleased(key);
	
	if (key == 'f' || key == 'F') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	// TODO: change mouse cursor if over map?
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	// TODO: check if we're over the map
	map.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	map.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	map.mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	// TODO: map.setSize(w,h)?
	map.width = w;
	map.height = h;
}

