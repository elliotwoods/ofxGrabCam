#pragma once
//
//  ofxGrabCam.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofRectangle.h"
#include "ofPixels.h"
#include "ofCamera.h"

#define OFXGRABCAM_SEARCH_WIDTH_PX 32
#define OFXGRABCAM_RESET_HOLD_MS 500
#define HAS_OFXGRABCAM

class ofxGrabCam : public ofCamera {
public:	
	ofxGrabCam();
	~ofxGrabCam();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); ///< Overrides virtual end
	void	reset();
	
	void			updateCursorWorld();
	const ofVec3f&	getCursorWorld() const;
	const ofVec3f&	getCursorProjected() const;
	
	void	setCursorDrawEnabled(bool);
	bool	getCursorDrawEnabled() const;
	void	toggleCursorDrawEnabled();

	void	setCursorDrawSize(float);
	float	getCursorDrawSize() const;
	
	void	setMouseActionsEnabled(bool);
	bool	getMouseActionsEnabled() const;
	void	toggleMouseActionsEnabled();
	
	void	setFixUpDirectionEnabled(bool);
	bool	getFixUpDirectionEnabled() const;
	void	toggleFixUpDirectionEnabled();
	
	void	setTrackballRadius(float);
	float	getTrackballRadius() const;
	
	void	setListenersEnabled(bool);
	bool	getListenersEnabled() const;
	
	////
	//events
	//
	void	update(ofEventArgs & args);
	void    mouseMoved(ofMouseEventArgs & args);
	void	mousePressed(ofMouseEventArgs & args);
	void	mouseReleased(ofMouseEventArgs & args);
	void	mouseDragged(ofMouseEventArgs & args);
	void	keyPressed(ofKeyEventArgs & args);
	void	keyReleased(ofKeyEventArgs & args);
	//
	////
	
protected:
	void addListeners();
	void removeListeners();

	void updateMouseCoords(const ofMouseEventArgs &, bool updateMouseOverViewport);
	void findCursor();

	struct {
		bool listenersAttached; // we manage this. only ever set by update or destructor

		struct {
			bool down;
			int button;
		} mouseDown;

		struct {
			bool h;
			bool r;
			uint64_t resetHoldStartTime;
		} keysDown;
	} inputState;

	struct {	
		ofQuaternion rotation;
		bool findMouseThisFrame;

		struct {
			ofVec3f projected; // within viewport pixels
			ofVec3f world;
			bool overViewport;
		} mouse;
	} tracking;

	struct {
		ofRectangle viewport;
		ofShortPixels sampleNeighbourhood;

		struct {
			GLint viewport[4];
			ofMatrix4x4 modelMatrix;
			ofMatrix4x4 projectionMatrix;
		} opengl;
	} view;

	struct {
		bool listenersEnabled;
		bool mouseActionsEnabled;
		bool fixUpDirection;
		float trackballRadius;

		struct {
			bool enabled;
			float size;
		} cursorDraw;

		struct {
			ofVec3f position;
			ofQuaternion rotation;
		} defaultView;
	} userSettings;

};