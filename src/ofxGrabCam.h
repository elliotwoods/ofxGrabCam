#pragma once
//
//  ofxGrabCam.h
//  ofxSketchupCamera
//
//  Created by Elliot Woods on 10/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofMain.h"

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
	//
	////
	
	
	////
	//camera
	void	findCursor();
	
	bool	initialised;
	
	bool	mouseDown;
	bool	pickCursorFlag;
	ofVec3f mouseP;
	ofVec3f mouseW;
	bool	drawCursor;
	float	drawCursorSize;
	
	GLint		viewport[4];
	GLdouble	matM[16], matP[16];
	
	ofQuaternion rotation;
	//
	////
};