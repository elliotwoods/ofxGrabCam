#pragma once
//
//  ofxGrabCam.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofMain.h"

class ofxGrabCam : public ofCamera {
public:	
	ofxGrabCam(bool useMouseListeners=true);
	~ofxGrabCam();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); ///< Overrides virtual end
	void	reset();
	
	const ofVec3f& getCursorWorld() { return mouseW; }
	const ofVec3f& getCursorProjected() { return mouseP; }
	
	void	setCursorDraw(bool enabled, float size=0.1);
	void	toggleCursorDraw();
	
	void	setFixUpwards(bool enabled);
	void	toggleFixUpwards();
	
	void	addListeners();
	void	removeListeners();
	
protected:
	
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
	bool	resetDown;
	float	resetDownBegin;
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
};