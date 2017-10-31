//
//  ofxGrabCam.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofxGrabCam.h"
#include "of3dGraphics.h"

#define OFXGRABCAM_SEARCH_WIDTH_PX 8
#define OFXGRABCAM_RESET_HOLD_MS 500

//--------------------------
ofxGrabCam::ofxGrabCam() {
	this->inputState.listenersAttached = false;
	this->inputState.mouseDown.down = false;
	this->inputState.mouseDown.button = 0;
	this->inputState.keysDown.h = false;
	this->inputState.keysDown.r = false;
	this->inputState.keysDown.resetHoldStartTime = 0;

	this->tracking.findMouseThisFrame = false;
	this->tracking.mouse.viewport.withinViewport = false;
	this->tracking.mouse.projectedDepth = 0.0f;

	for (int i = 0; i < 4; i++) {
		this->view.opengl.viewport[i] = 0;
	}

	this->userSettings.listenersEnabled = true;
	this->userSettings.mouseActionsEnabled = true;
	this->userSettings.fixUpDirection = false;
	this->userSettings.trackballRadius = 0.2f;
	this->userSettings.cursorDraw.enabled = false;
	this->userSettings.cursorDraw.size = 0.02f;

	//reset camera orientation and zoom
	this->reset();
}

//--------------------------
ofxGrabCam::~ofxGrabCam() {
	//note that older versions of oF (before october 2015) sometimes have crashes when removing listeners 
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
		//cursorDraw.size is in normalized screen coords
		const auto viewToCursor = this->tracking.mouse.world * this->getModelViewMatrix();
		const auto viewHeightAtCursor = tan(this->getFov() / 2.0f * DEG_TO_RAD) * viewToCursor.z * 2.0f;
		auto size = this->userSettings.cursorDraw.size * viewHeightAtCursor;
		
		const auto pxToWorldAtCursor = viewHeightAtCursor / this->view.viewport.getHeight();
		
		ofPushStyle();
		ofPushMatrix();
		{
			ofTranslate(this->tracking.mouse.world);

			//draw line inner
			ofSetColor(255);
			ofSetLineWidth(1.0f);
			ofDrawLine(-size, 0, 0, size, 0, 0);
			ofDrawLine(0, -size, 0, 0, size, 0);
			ofDrawLine(0, 0, -size, 0, 0, size);
			
			//draw line outer
			glPolygonOffset(1.0f, -1.0f);
			glEnable(GL_POLYGON_OFFSET_LINE);
			ofSetColor(0);
			ofSetLineWidth(4.0f);
			size += pxToWorldAtCursor * 4.0f;
			ofDrawLine(-size, 0, 0, size, 0, 0);
			ofDrawLine(0, -size, 0, 0, size, 0);
			ofDrawLine(0, 0, -size, 0, 0, size);
		}
		ofPopMatrix();
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
				ofTranslate(this->tracking.mouse.viewport.position);
				ofFill();
				ofSetColor(50, 10, 10);
				ofDrawRectangle(20, 20, 80, 40);

				stringstream ss;
				ss << "x: " << ofToString(this->tracking.mouse.world.x, 3) << endl;
				ss << "y: " << ofToString(this->tracking.mouse.world.y, 3) << endl;
				ss << "z: " << ofToString(this->tracking.mouse.world.z, 3) << endl;

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
ofVec3f ofxGrabCam::getCursorProjected() const {
	return ofVec3f(this->tracking.mouse.viewport.position.x,
		this->tracking.mouse.viewport.position.y,
		this->tracking.mouse.projectedDepth);
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
	if (this->userSettings.cursorDraw.enabled && this->tracking.mouse.viewport.withinViewport && !this->inputState.mouseDown.down) {
		//if the cursor is being drawn then always update the cursor world position (e.g. scene might be moving)
		this->tracking.findMouseThisFrame = true;
	}
}

//--------------------------
void ofxGrabCam::mouseMoved(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}

	// cache mouse position
	this->tracking.mouse.viewport = this->getMouseInViewport(args);
}

//--------------------------
void ofxGrabCam::mousePressed(ofMouseEventArgs & args) {
	if (!this->userSettings.mouseActionsEnabled) {
		return;
	}

	// if a mouse button is already down, then ignore the new button
	if (this->inputState.mouseDown.down) {
		return;
	}

	// cache mouse position
	this->tracking.mouse.viewport = this->getMouseInViewport(args);
	
	// if mouse goes down in our viewport then let's take it
	if (this->tracking.mouse.viewport.withinViewport) {
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

	auto updatedMouse = this->getMouseInViewport(args);

	//calculate mouse movement this frame
	auto mouseMovement = updatedMouse.position - this->tracking.mouse.viewport.position;

	//don't copy all parameters to the cached mouse, keep the z and withinViewport state as before
	this->tracking.mouse.viewport.position = updatedMouse.position;
	
	const auto cameraPosition = ofCamera::getPosition();
	const auto cameraUpDirection = ofCamera::getUpDir();
	const auto cameraSideDirection= ofCamera::getSideDir();
	float aspectRatio = float(this->view.viewport.getWidth()) / float(this->view.viewport.getHeight());
	
	const auto cameraToMouse = this->tracking.mouse.world - cameraPosition;

	enum Action {
		Orbit,
		Pan,
		Dolly,
		None
	};
	Action action = Action::None;

	if (this->inputState.keysDown.h) {
		action = Action::Pan;
	} else {
		switch (this->inputState.mouseDown.button) {
		case 0:
			action = Action::Orbit;
			break;
		case 1:
			action = Action::Pan;
			break;
		case 2:
			action = Action::Dolly;
			break;
		default:
			break;
		}
	}

	switch (action) {
	case Action::Orbit:
	{
		auto arcEnd = ofVec3f(mouseMovement.x, -mouseMovement.y, -this->userSettings.trackballRadius * this->view.viewport.getWidth()).getNormalized();
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
		break;
	}
	case Action::Pan:
	{
		float distanceToMouse = cameraToMouse.length();
		auto halfFov = (ofCamera::getFov() / 2.0) * DEG_TO_RAD;
		float grad = tan(halfFov) * 2.0f;
		ofCamera::move(mouseMovement.x / this->view.viewport.getWidth()  *-cameraSideDirection * distanceToMouse * aspectRatio * grad);
		ofCamera::move(mouseMovement.y / this->view.viewport.getHeight() * cameraUpDirection   * distanceToMouse * grad);
		break;
	}
	case Action::Dolly:
	{
		ofCamera::move(2 * cameraToMouse * mouseMovement.y / this->view.viewport.getHeight());
		break;
	}
	case Action::None:
	default:
		break;
	}
}

//--------------------------
void ofxGrabCam::keyPressed(ofKeyEventArgs & args) {
	if (args.key == 'r') {
		if (!this->inputState.keysDown.r) {
			this->inputState.keysDown.resetHoldStartTime = ofGetElapsedTimeMillis();
		}
		else if (ofGetElapsedTimeMillis() - this->inputState.keysDown.resetHoldStartTime > OFXGRABCAM_RESET_HOLD_MS) {
			this->reset();
		}

		this->inputState.keysDown.r = true;
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
const ofShortPixels & ofxGrabCam::getSampleNeighbourhood() const {
	return this->view.sampleNeighbourhood;
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
ofxGrabCam::MouseInViewport ofxGrabCam::getMouseInViewport(const ofMouseEventArgs & args) {
	MouseInViewport mouseInViewport = {
		ofVec2f(args.x - this->view.viewport.x, args.y - this->view.viewport.y),
		this->view.viewport.inside(args.x, args.y)
	};
	return mouseInViewport;
}

//--------------------------
void ofxGrabCam::findCursor() {
	GLint mouseViewportX = this->tracking.mouse.viewport.position.x;
	GLint mouseViewportY = this->view.viewport.height - 1 - this->tracking.mouse.viewport.position.y;

	const auto nearPlaneZ = (unsigned short) 32768;
	const auto farPlaneZ = (unsigned short) 65535;

	unsigned short z = farPlaneZ;

	//sampleRect will be in OpenGL's viewport coordinates
	auto sampleRect = ofRectangle(-OFXGRABCAM_SEARCH_WIDTH_PX / 2, -OFXGRABCAM_SEARCH_WIDTH_PX / 2, OFXGRABCAM_SEARCH_WIDTH_PX, OFXGRABCAM_SEARCH_WIDTH_PX);
	sampleRect.x += mouseViewportX;
	sampleRect.y += mouseViewportY;
	auto cropRect = ofRectangle(0, 0, this->view.viewport.width, this->view.viewport.height);
	sampleRect = sampleRect.getIntersection(cropRect);

	//this should always be true since findCursor is only called whilst cursor is inside viewport
	if (sampleRect.width > 0 && sampleRect.height > 0) {
		//check if we need to reallocate our local buffer for depth pixels
		if (this->view.sampleNeighbourhood.getWidth() != sampleRect.getWidth() || this->view.sampleNeighbourhood.getHeight() != sampleRect.getHeight()) {
			this->view.sampleNeighbourhood.allocate(sampleRect.getWidth(), sampleRect.getHeight(), OF_IMAGE_GRAYSCALE);
		}

		//sample depth pixels in the neighbourhood of the mouse
		glReadPixels(sampleRect.x, sampleRect.y, sampleRect.width, sampleRect.height, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, this->view.sampleNeighbourhood.getData());

		//pick the closest pixel to use as a sample
		for (auto & pixel : this->view.sampleNeighbourhood) {
			//check that it's valid before using it
			if (pixel != nearPlaneZ && pixel != farPlaneZ) {
				z = MIN(pixel, z);
			}
		}
	}

	//check we're still looking at the near/far plane before updating the mouse distance
	if (z != nearPlaneZ && z != farPlaneZ) {
		this->tracking.mouse.projectedDepth = ((float)z / (float)USHRT_MAX) * 2.0f - 1.0f;
	}
	
	//find mouse coordinates
	auto mouseProjected = this->getCursorProjected();
	mouseProjected += this->view.viewport.getTopLeft();
	this->tracking.mouse.world = this->screenToWorld(mouseProjected, this->view.viewport);
}