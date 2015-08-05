//
//  ofxGrabCam.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofxGrabCam.h"

//--------------------------
ofxGrabCam::ofxGrabCam() {
	this->inputState.listenersAttached = false;
	this->inputState.mouseDown.down = false;
	this->inputState.mouseDown.button = 0;
	this->inputState.keysDown.h = false;
	this->inputState.keysDown.r = false;
	this->inputState.keysDown.resetHoldStartTime = 0;

	this->tracking.findMouseThisFrame = false;
	tracking.mouse.overViewport = true;

	for (int i = 0; i < 4; i++) {
		this->view.opengl.viewport[i] = 0;
	}

	this->userSettings.listenersEnabled = true;
	this->userSettings.mouseActionsEnabled = true;
	this->userSettings.fixUpDirection = false;
	this->userSettings.trackballRadius = 0.5f;
	this->userSettings.cursorDraw.enabled = false;
	this->userSettings.cursorDraw.size = 0.1f;

	//reset camera orientation and zoom
	this->reset();
}

//--------------------------
ofxGrabCam::~ofxGrabCam() {
	//removing events actually seems to upset something
	this->removeListeners();
}

//--------------------------
void ofxGrabCam::begin(ofRectangle viewport) {
	//check listeners are in order
	if (this->inputState.listenersAttached) {
		if (!this->userSettings.listenersEnabled) {
			this->removeListeners();
		}
	}
	else {
		if (this->userSettings.listenersEnabled) {
			this->addListeners();
		}
	}

	glEnable(GL_DEPTH_TEST);
	this->view.viewport = viewport;
	ofCamera::begin(viewport);
	
	glGetFloatv(GL_PROJECTION_MATRIX, this->view.opengl.projectionMatrix.getPtr());
	glGetFloatv(GL_MODELVIEW_MATRIX, this->view.opengl.modelMatrix.getPtr());
	glGetIntegerv(GL_VIEWPORT, this->view.opengl.viewport);

	ofPushMatrix();
}

//--------------------------
void ofxGrabCam::end() {
	// optimistically, we presume there's no stray push/pops inbetween
	ofPopMatrix();

	if (this->tracking.findMouseThisFrame) {
		findCursor();
		this->tracking.findMouseThisFrame = false; //lower the flag
	}
	
	//--
	// Draw the 3D cursor
	//--
	//
	// this has to happen after all drawing + findCursor()
	// but before camera.end()
	if (this->userSettings.cursorDraw.enabled) {
		ofPushStyle();
		ofSetColor(0, 0, 0);
		ofSphere(this->tracking.mouse.world, this->userSettings.cursorDraw.size);
		ofPopStyle();
	}
	//
	//--

	ofCamera::end();
	glDisable(GL_DEPTH_TEST);
	
	//--
	// Draw the 2D cursor info box
	//--
	//
	if (this->userSettings.cursorDraw.enabled) {
		ofPushStyle();
		{
			ofPushMatrix();
			{
				ofTranslate(this->tracking.mouse.projected.x, this->tracking.mouse.projected.y);
				ofFill();
				ofSetColor(50, 10, 10);
				ofDrawRectangle(20, 20, 80, 40);

				stringstream ss;
				ss << "x: " << ofToString(this->tracking.mouse.world.x, 2) << endl;
				ss << "y: " << ofToString(this->tracking.mouse.world.y, 2) << endl;
				ss << "z: " << ofToString(this->tracking.mouse.world.z, 2) << endl;

				ofSetColor(255, 255, 255);
				ofDrawBitmapString(ss.str(), 30, 30);
			}
			ofPopMatrix();
		}
		ofPopStyle();
	}
	//
	//--
}

//--------------------------
void ofxGrabCam::reset() {
	ofCamera::setNearClip(0.1);
	ofCamera::setFarClip(1000.0f);
	this->setPosition(1.0f, 1.0f, -1.0f);
	this->lookAt(ofVec3f());
}

//--------------------------
void ofxGrabCam::updateCursorWorld() {
	this->findCursor();
}

//--------------------------
const ofVec3f & ofxGrabCam::getCursorWorld() const {
	return this->tracking.mouse.world;
}

//--------------------------
const ofVec3f & ofxGrabCam::getCursorProjected() const {
	return this->tracking.mouse.projected;
}

//--------------------------
void ofxGrabCam::setCursorDrawEnabled(bool enabled) {
	this->userSettings.cursorDraw.enabled = enabled;
}

//--------------------------
bool ofxGrabCam::getCursorDrawEnabled() const {
	return this->userSettings.cursorDraw.enabled;
}

//--------------------------
void ofxGrabCam::toggleCursorDrawEnabled() {
	this->userSettings.cursorDraw.enabled ^= true;
}

//--------------------------
void ofxGrabCam::setCursorDrawSize(float cursorDrawSize) {
	this->userSettings.cursorDraw.size = cursorDrawSize;
}

//--------------------------
float ofxGrabCam::getCursorDrawSize() const {
	return this->userSettings.cursorDraw.size;
}

//--------------------------
void ofxGrabCam::setMouseActionsEnabled(bool enabled) {
	this->userSettings.mouseActionsEnabled = enabled;
}

//--------------------------
bool ofxGrabCam::getMouseActionsEnabled() const {
	return this->userSettings.mouseActionsEnabled;
}

//--------------------------
void ofxGrabCam::toggleMouseActionsEnabled() {
	this->userSettings.mouseActionsEnabled ^= true;
}

//--------------------------
void ofxGrabCam::setFixUpDirectionEnabled(bool enabled) {
	this->userSettings.fixUpDirection = enabled;
}

//--------------------------
bool ofxGrabCam::getFixUpDirectionEnabled() const{
	return this->userSettings.fixUpDirection;
}

//--------------------------
void ofxGrabCam::toggleFixUpDirectionEnabled() {
	this->userSettings.fixUpDirection ^= true;
}

//--------------------------
void ofxGrabCam::setTrackballRadius(float trackballRadius) {
	this->userSettings.trackballRadius = trackballRadius;
}

//--------------------------
float ofxGrabCam::getTrackballRadius() const {
	return this->userSettings.trackballRadius;
}

//--------------------------
void ofxGrabCam::setListenersEnabled(bool listenersEnabled) {
	this->userSettings.listenersEnabled = listenersEnabled;
}

//--------------------------
bool ofxGrabCam::getListenersEnabled() const {
	return this->userSettings.listenersEnabled;
}

//--------------------------
void ofxGrabCam::update(ofEventArgs &args) {
	if (this->userSettings.cursorDraw.enabled && this->tracking.mouse.overViewport && !this->inputState.mouseDown.down) {
		//if the cursor is being drawn then always update the cursor world position (e.g. scene might be moving)
		this->tracking.findMouseThisFrame = true;
	}
}

//--------------------------
void ofxGrabCam::mouseMoved(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}

	this->updateMouseCoords(args, true);
}

//--------------------------
void ofxGrabCam::mousePressed(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}

	//if a mouse button is already down, then ignore the new button
	if (this->inputState.mouseDown.down) {
		return;
	}

	this->updateMouseCoords(args, true);
	
	//if mouse goes down in our viewport then let's take it
	if (this->tracking.mouse.overViewport) {
		this->tracking.findMouseThisFrame = true;

		this->inputState.mouseDown.down = true;
		this->inputState.mouseDown.button = args.button;
	}
}

//--------------------------
void ofxGrabCam::mouseReleased(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}
	
	//clear the mouse down flag if it matches the button which we're considering
	if (this->inputState.mouseDown.button == args.button) {
		this->inputState.mouseDown.down = false;
	}
}

//--------------------------
void ofxGrabCam::mouseDragged(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}
	
	if (!this->inputState.mouseDown.down) {
		//we didn't go down inside the viewport, so ignore the drag
		return;
	}

	//calculate mouse movement this frame
	float dx = (args.x - this->tracking.mouse.projected.x) / this->view.viewport.getWidth();
	float dy = (args.y - this->tracking.mouse.projected.y) / this->view.viewport.getHeight();

	this->updateMouseCoords(args, false);
	
	const auto cameraPosition = ofCamera::getPosition();
	const auto cameraUpDirection = ofCamera::getUpDir();
	const auto cameraSideDirection= ofCamera::getSideDir();
	float aspectRatio = float(this->view.viewport.getWidth()) / float(this->view.viewport.getHeight());
	
	const auto cameraToMouse = this->tracking.mouse.world - cameraPosition;

	if (this->inputState.keysDown.h) {
		//pan
		float distanceToMouse = cameraToMouse.length();
		//ofCamera::getFov() doesn't exist!! (well it does now actually!)
		ofCamera::move(dx * -cameraSideDirection * distanceToMouse * aspectRatio);
		ofCamera::move(dy * cameraUpDirection * distanceToMouse);
	} else {
		if (this->inputState.mouseDown.button == 0) {
			//orbit
			auto arcEnd = ofVec3f(dx, -dy, -this->userSettings.trackballRadius).getNormalized();
			ofQuaternion rotateCamera;
			auto cameraOrientation = this->getOrientationQuat();
			rotateCamera.makeRotate(cameraOrientation * ofVec3f(0.0f, 0.0f, -1.0f), cameraOrientation * arcEnd);
			
			if (this->userSettings.fixUpDirection) {
				ofQuaternion rotToUp;
				ofVec3f sideDir = ofCamera::getSideDir() * rotateCamera;
				rotToUp.makeRotate(sideDir, sideDir * ofVec3f(1.0, 0.0f, 1.0f));
				rotateCamera *= rotToUp;
			}
			
			this->setOrientation(cameraOrientation * rotateCamera);
			ofCamera::setPosition((-cameraToMouse) * rotateCamera + this->tracking.mouse.world);
		} else {
			//dolly
			ofCamera::move(2 * cameraToMouse * dy);
		}
	}
}

//--------------------------
void ofxGrabCam::keyPressed(ofKeyEventArgs & args) {
	if (args.key == 'r') {
		if (this->inputState.keysDown.resetHoldStartTime == 0) {
			this->inputState.keysDown.resetHoldStartTime = ofGetElapsedTimeMillis();
		}
		else if (ofGetElapsedTimeMillis() - this->inputState.keysDown.resetHoldStartTime > OFXGRABCAM_RESET_HOLD_MS) {
			this->reset();
		}
	}

	if (args.key == 'h') {
		this->inputState.keysDown.h = true;
	}
}


//--------------------------
void ofxGrabCam::keyReleased(ofKeyEventArgs & args) {
	if (args.key == 'h') {
		this->inputState.keysDown.h = false;
	}
	
	if (args.key == 'r') {
		this->inputState.keysDown.r = false;
	}
}

//--------------------------
void ofxGrabCam::addListeners() {
	this->removeListeners();

	ofAddListener(ofEvents().update, this, &ofxGrabCam::update);
	ofAddListener(ofEvents().mouseMoved, this, &ofxGrabCam::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &ofxGrabCam::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxGrabCam::mouseReleased);
	ofAddListener(ofEvents().mouseDragged, this, &ofxGrabCam::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &ofxGrabCam::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxGrabCam::keyReleased);

	this->inputState.listenersAttached = true;
}

//--------------------------
void ofxGrabCam::removeListeners() {
	if (this->inputState.listenersAttached) {
		ofRemoveListener(ofEvents().update, this, &ofxGrabCam::update);
		ofRemoveListener(ofEvents().mouseMoved, this, &ofxGrabCam::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &ofxGrabCam::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxGrabCam::mouseReleased);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxGrabCam::mouseDragged);
		ofRemoveListener(ofEvents().keyPressed, this, &ofxGrabCam::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &ofxGrabCam::keyReleased);

		this->inputState.listenersAttached = false;
	}
}

//--------------------------
void ofxGrabCam::updateMouseCoords(const ofMouseEventArgs & args, bool updateMouseOverViewport) {
	this->tracking.mouse.projected.x = args.x - this->view.viewport.x;
	this->tracking.mouse.projected.y = args.y - this->view.viewport.y;

	if (updateMouseOverViewport) {
		this->tracking.mouse.overViewport = this->view.viewport.inside(args.x, args.y);
	}
}

//--------------------------
void ofxGrabCam::findCursor() {
	GLint mouseScreenX = this->tracking.mouse.projected.x;
	GLint mouseScreenY = this->view.viewport.height - 1 - this->tracking.mouse.projected.y;
	unsigned short z;

	const auto nearPlaneZ = (unsigned short) 32768;
	const auto farPlaneZ = (unsigned short) 65535;

	// read the position under the cursor
	glReadPixels(mouseScreenX, mouseScreenY, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, &z);

	// if nothing found, sample a neighbourhood
	if (z == nearPlaneZ|| z == farPlaneZ) {
		auto sampleRect = ofRectangle(-OFXGRABCAM_SEARCH_WIDTH_PX / 2, -OFXGRABCAM_SEARCH_WIDTH_PX / 2, OFXGRABCAM_SEARCH_WIDTH_PX, OFXGRABCAM_SEARCH_WIDTH_PX);
		sampleRect.x += mouseScreenX;
		sampleRect.y += mouseScreenY;
		auto cropRect = ofRectangle(0, 0, this->view.viewport.width, this->view.viewport.height);
		sampleRect = sampleRect.getIntersection(cropRect);

		if (sampleRect.width > 0 && sampleRect.height > 0) {
			if (this->view.sampleNeighbourhood.getWidth() != sampleRect.getWidth() || this->view.sampleNeighbourhood.getHeight() != sampleRect.getHeight()) {
				this->view.sampleNeighbourhood.allocate(sampleRect.getWidth(), sampleRect.getHeight(), OF_IMAGE_GRAYSCALE);
			}
			glReadPixels(sampleRect.x, sampleRect.y, sampleRect.width, sampleRect.height, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, this->view.sampleNeighbourhood.getData());
			for (auto & pixel : this->view.sampleNeighbourhood) {
				if (pixel != nearPlaneZ && pixel != farPlaneZ) {
					z = pixel;
					break;
				}
			}
		}
	}

	//check we're still looking at the near/far plane before updating the mouse distance
	if (z != nearPlaneZ && z != farPlaneZ) {
		this->tracking.mouse.projected.z = ((float)z / (float)USHRT_MAX) * 2.0f - 1.0f;
	}
	
	//find mouse coordinate
	this->tracking.mouse.world = this->screenToWorld(this->tracking.mouse.projected);
}