#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	for (int i=0; i<300; i++)
	{
		auto position = glm::vec3(
			ofRandom(-10, 10),
			ofRandom(-10, 10),
			ofRandom(-10, 10)
		);
		
		auto color = ofColor(
			ofRandom(100) + 100,
			ofRandom(100) + 100,
			ofRandom(100) + 100
		);

		this->spheres.push_back({
			position,
			color
		});
	}
	
	ofBackground(50, 50, 50);
	ofEnableSmoothing();
	
	// You might need to do this in the future if openFrameworks changes currently it's done automatically in the constructor, but this may be too early for openFrameworks (if the current situation changes)
	// camera.init();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	//--
	//draw 3d bits
	//--
	//
	camera.begin();
	this->drawScene();
	camera.end();
	//
	//--


	//--
	//draw 2d text notes on top
	//--
	//
	ofPushStyle();
	{
		ofSetColor(0, 0, 0);

		ofDrawBitmapStringHighlight("ofxGrabCam Example", 30, 30, ofColor::darkSlateGray, ofColor::white);
		ofDrawBitmapString("ofxGrabCam Example", 31, 30); //embolden

		stringstream message;
		message << "ofxGrabCam keys:" << endl;
		message << "Drag with left mouse to orbit" << endl;
		message << "Drag with right mouse to zoom" << endl;
		message << "Hold 'h' and drag with left mouse to pan" << endl;
		message << "Press 'r' to reset" << endl;
		message << endl;
		message << "This example keys:" << endl;
		message << "Press 'c' to toggleCursorDraw" << endl;
		message << "Press 'u' to  toggleFixUpwards" << endl;
		message << "Press 's' to save camera pose to savedPose" << endl;
		message << "Press 'l' to load camera pose from savedPose";

		ofDrawBitmapStringHighlight(message.str(), 30, 60, ofColor::darkSlateGray, ofColor::white);
	}
	ofPopStyle();
	//
	//--



	//--
	// draw debug preview
	//--
	//
	// NOTE : this will not be updated if either:
	//	a) the cursor is outside the screen
	//	b) the cursor is directly over something (in which case we don't sample the neighbourhood)
	//
	const auto & depthPixelsAroundCursor = this->camera.getSampleNeighbourhood();
	if (depthPixelsAroundCursor.isAllocated()) {
		ofShortImage preview = depthPixelsAroundCursor;
		preview.update();
		preview.draw(30, 200, 100, 100);
	}
	//
	//--
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	ofSetColor(255, 100, 100);
	ofDrawGrid(1.0f, 10.0f, true);
	for (auto & sphere : this->spheres)
	{
		ofSetColor(sphere.color);
		ofSphere(sphere.position, 0.3f);
	}

	//demonstrate resiliance to stray matrices
	ofRotate(ofRandom(360), 0, 1, 0);
	ofScale(ofRandom(30), ofRandom(30));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'c') {
		camera.toggleCursorDrawEnabled();
	}

	if (key == 'u') {
		camera.toggleFixUpDirectionEnabled();
	}

	if (key == 's') {
		//savedPose = camera.getGlobalTransformMatrix();
	}

	if (key == 'l') {
		//camera.setTransformMatrix(savedPose);
	}
}

