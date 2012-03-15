//
//  ofxGrabCam.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofxGrabCam.h"

//--------------------------
ofxGrabCam::ofxGrabCam(bool useMouseListeners) {
	this->mouseDown = false;
	this->altDown = false;
	this->handDown = false;
	this->resetDown = false;
	this->pickCursorFlag = false;
	this->drawCursor = false;
	this->drawCursorSize = 0.1f;
	this->fixUpwards = true;

	ofCamera::setNearClip(0.1);
	addListeners();
	reset();
}

//--------------------------
ofxGrabCam::~ofxGrabCam() {
	//removing events actually seems to upset something
	//removeListeners();
}

//--------------------------
void ofxGrabCam::begin(ofRectangle viewport) {
	glEnable(GL_DEPTH_TEST);	
	viewportRect = viewport;
	ofCamera::begin(viewport);
	ofPushMatrix();
	
	glGetDoublev(GL_PROJECTION_MATRIX, this->matP);
	glGetDoublev(GL_MODELVIEW_MATRIX, this->matM);
	glGetIntegerv(GL_VIEWPORT, this->viewport);
}

//--------------------------
void ofxGrabCam::end() {
	
	//optimistically, we presume there's no stray push/pops
	ofPopMatrix();
	
	if (pickCursorFlag || !mouseDown) {
		findCursor();
		pickCursorFlag = false;
	}
	
	// this has to happen after all drawing + findCursor()
	// but before camera.end()
	if (drawCursor) {
		ofPushStyle();
		ofSetColor(0, 0, 0);
		ofSphere(mouseW.x, mouseW.y, mouseW.z, drawCursorSize);
		ofPopStyle();
	}
	
	ofCamera::end();
	glDisable(GL_DEPTH_TEST);
	
	if (drawCursor && viewportRect.inside(mouseP)) {
		ofPushStyle();
		ofFill();
		ofSetColor(50, 10, 10);
		ofRect(mouseP.x + 20, mouseP.y + 20, 80, 40);
		
		stringstream ss;
		ss << "x: " << ofToString(mouseW.x, 2) << endl;
		ss << "y: " << ofToString(mouseW.y, 2) << endl;
		ss << "z: " << ofToString(mouseW.z, 2) << endl;
		
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(ss.str(), mouseP.x + 30, mouseP.y + 30);
		
		ofPopStyle();
	}
}

//--------------------------
void ofxGrabCam::reset() {
	ofCamera::resetTransform();
	ofCamera::setPosition(2.0f, 1.0f, 2.0f);
	ofCamera::lookAt( ofVec3f(0.0f, 0.0f, 0.0f) );
}

//--------------------------
void ofxGrabCam::setCursorDraw(bool enabled, float size) {
	this->drawCursor = enabled;
	this->drawCursorSize = size;
}

//--------------------------
void ofxGrabCam::toggleCursorDraw() {
	this->drawCursor ^= true;
}

//--------------------------
void ofxGrabCam::setFixUpwards(bool enabled) {
	fixUpwards = enabled;
}

//--------------------------
void ofxGrabCam::toggleFixUpwards() {
	fixUpwards ^= true;
}

//--------------------------
void ofxGrabCam::addListeners() {
	ofAddListener(ofEvents.update, this, &ofxGrabCam::update);
	ofAddListener(ofEvents.mouseMoved, this, &ofxGrabCam::mouseMoved);
	ofAddListener(ofEvents.mousePressed, this, &ofxGrabCam::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxGrabCam::mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &ofxGrabCam::mouseDragged);
	ofAddListener(ofEvents.keyPressed, this, &ofxGrabCam::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxGrabCam::keyReleased);

	initialised = true;
}

//--------------------------
void ofxGrabCam::removeListeners() {
	if (!initialised)
		return;
	
	ofRemoveListener(ofEvents.update, this, &ofxGrabCam::update);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxGrabCam::mouseMoved);
	ofRemoveListener(ofEvents.mousePressed, this, &ofxGrabCam::mousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxGrabCam::mouseReleased);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxGrabCam::mouseDragged);
	ofRemoveListener(ofEvents.keyPressed, this, &ofxGrabCam::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &ofxGrabCam::keyReleased);
	
	initialised = false;
}

//--------------------------
void ofxGrabCam::update(ofEventArgs &args) {
	
}

//--------------------------
void ofxGrabCam::mouseMoved(ofMouseEventArgs &args) {
	mouseP.x = args.x;
	mouseP.y = args.y;
}

//--------------------------
void ofxGrabCam::mousePressed(ofMouseEventArgs &args) {
	if (!viewportRect.inside(args.x, args.y))
		return;
	
	mouseP.x = args.x;
	mouseP.y = args.y;
	
	if (viewportRect.inside(args.x, args.y)) {
		if (!mouseDown)
			pickCursorFlag = true;
		mouseDown = true;
	} else {
		mouseDown = false;
	}
}

//--------------------------
void ofxGrabCam::mouseReleased(ofMouseEventArgs &args) {
	mouseDown = false;
}

//--------------------------
void ofxGrabCam::mouseDragged(ofMouseEventArgs &args) {
	if (!this->mouseDown)
		return;
	
	float dx = (args.x - mouseP.x) / ofGetViewportWidth();
	float dy = (args.y - mouseP.y) / ofGetViewportHeight();
	
	mouseP.x = args.x;
	mouseP.y = args.y;
	
	if (mouseP.z == 1.0f)
		mouseP.z = 0.5f;
	
	ofVec3f p = ofCamera::getPosition();
	ofVec3f uy = ofCamera::getUpDir();
	ofVec3f ux = ofCamera::getSideDir();
	float ar = float(ofGetViewportWidth()) / float(ofGetViewportHeight());
	
	if (handDown) {
		//pan
		float d = (p - mouseW).length();
		//ofCamera::getFov() doesn't exist!!
		ofCamera::move(dx * -ux * d * ar);
		ofCamera::move(dy * uy * d);
	} else {
		if (args.button==0 && !altDown) {
			//orbit
			rotation.makeRotate(dx * 180 * ar, -uy, dy * 180, -ux, 0, ofVec3f(0,0,1));
			
			if (fixUpwards) {
				ofQuaternion rotToUp;
				ofVec3f sideDir = ofCamera::getSideDir() * rotation;
				rotToUp.makeRotate(sideDir, sideDir * ofVec3f(1.0f, 0, 1.0f));
				rotation *= rotToUp;
			}
			
			ofCamera::setPosition((p - mouseW) * rotation + mouseW);
			ofCamera::rotate(rotation);
		} else {
			//dolly
			ofCamera::move(2 * (mouseW - p) * dy);
		}
	}
}

//--------------------------
void ofxGrabCam::keyPressed(ofKeyEventArgs &args) {
	if (args.key == 'r') {
		if (!resetDown) {
			resetDownBegin = ofGetElapsedTimef();
			resetDown = true;
		} else {
			if (ofGetElapsedTimef() - resetDownBegin > 0.5f) {
				reset();
				resetDown = false;
			}
		}
	}
	
	if (args.key == 'h')
		handDown = true;
	
	if (args.key == OF_KEY_ALT)
		altDown = true;
}


//--------------------------
void ofxGrabCam::keyReleased(ofKeyEventArgs &args) {
	if (args.key == 'h')
		handDown = false;

	if (args.key == 'r')
		resetDown = false;
	
	if (args.key == OF_KEY_ALT)
		altDown = false;
}


//--------------------------
//in normalised screen coords -0.5>0.5
#define OFXGRABCAM_SEARCH_WIDTH 0.3f
#define OFXGRABCAM_SEARCH_MAX_ITERATIONS 300
#define OFXGRABCAM_SEARCH_WINDINGS 3.0f
void ofxGrabCam::findCursor() {
	//read z value from depth buffer at mouse coords
	glReadPixels(mouseP.x, ofGetHeight()-1-mouseP.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseP.z);
	
	//if we get nothing, scatter until we get something
	//we search in a spiral until we hit something
	if (mouseP.z == 1.0f) {
		float sx, sy; // search this spot in screen space
		float r, theta; // search is in polar coords
		for (int iteration=0; iteration < OFXGRABCAM_SEARCH_MAX_ITERATIONS; iteration++) {
			r = OFXGRABCAM_SEARCH_WIDTH * float(iteration) / float(OFXGRABCAM_SEARCH_MAX_ITERATIONS);
			theta = OFXGRABCAM_SEARCH_WINDINGS * 2 * PI * float(iteration) / float(OFXGRABCAM_SEARCH_MAX_ITERATIONS);
			sx = ofGetWidth() * r * cos(theta) + mouseP.x;
			sy = ofGetHeight() * r * sin(theta) + mouseP.y;
			
			if (!viewportRect.inside(sx, sy))
				continue;
			
			glReadPixels(sx, ofGetViewportHeight()-1-sy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseP.z);
			
			if (mouseP.z != 1.0f)
				break;
		}
	}
	
	if (mouseP.z == 1.0f)
		return;
	
	GLdouble c[3];
	
	gluUnProject(mouseP.x, ofGetHeight()-1-mouseP.y, mouseP.z, matM, matP, viewport, c, c+1, c+2);
	
	mouseW.x = c[0];
	mouseW.y = c[1];
	mouseW.z = c[2];
}