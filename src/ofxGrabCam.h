#pragma once
//
//  ofxGrabCam.h
//  ofxSketchupCamera
//
//  Created by Elliot Woods on 10/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofMain.h"

#include "ofxDragNode.h"
#include "ofxInteractiveNode.h"

//in normalised screen coords -0.5>0.5
#define OFXGRABCAM_SEARCH_WIDTH 1.0f
#define OFXGRABCAM_SEARCH_MAX_ITERATIONS 100
#define OFXGRABCAM_SEARCH_WINDINGS 10.0f

class ofxGrabCam : public ofCamera {
public:	
	ofxGrabCam();
	~ofxGrabCam();
	
	void	init();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); ///< Overrides virtual end
	void	reset();
	
	const ofVec3f& getCursorWorld() { return mouseW; }
	const ofVec3f& getCursorProjected() { return mouseP; }
	
	void	setCursorDraw(bool enabled, float size=0.1);
	void	toggleCursorDraw();
	
	void	setFixUpwards(bool enabled);
	void	toggleFixUpwards();

	static void addInteractiveNode(ofxInteractiveNode& node);
	static void removeInteractiveNode(ofxInteractiveNode& node);
	
protected:
	
	////
	//events
	void	addListeners();
	void	removeListeners();
	//
	void	update(ofEventArgs & args);
	void    mouseMoved(ofMouseEventArgs & args);
	void	mousePressed(ofMouseEventArgs & args);
	void	mouseReleased(ofMouseEventArgs & args);
	void	mouseDragged(ofMouseEventArgs & args);
	void	keyPressed(ofKeyEventArgs & args);
	void	keyReleased(ofKeyEventArgs & args);
	//
	bool	initialised;
	//
	////
	
	
	////
	//cursor
	void	findCursor();
	//
	bool	mouseDown;
	bool	handDown;
	bool	altDown;
	bool	pickCursorFlag;
	ofVec3f mouseP;
	ofVec3f mouseW;
	bool	drawCursor;
	float	drawCursorSize;
	//
	////
	
	
	////
	//view
	ofRectangle viewportRect;
	//
	GLint		viewport[4];
	GLdouble	matM[16], matP[16];
	//
	ofQuaternion rotation;
	//
	bool		fixUpwards;
	//
	////
	
	
	////
	//interaction
	//
	static vector<ofxInteractiveNode*> nodes;
	//
	////
};