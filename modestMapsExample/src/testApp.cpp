#include "testApp.h"
#include "OpenStreetMapProvider.h"

//--------------------------------------------------------------
void testApp::setup(){
	map.setup(new OpenStreetMapProvider(), (double)ofGetWidth(), (double)ofGetHeight());
}

//--------------------------------------------------------------
void testApp::update(){
	map.update();
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
}

