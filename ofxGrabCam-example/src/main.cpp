#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings windowSettings;
	windowSettings.setGLVersion(3, 1);
	windowSettings.windowMode = OF_WINDOW;

	ofCreateWindow(windowSettings);

	ofRunApp(new ofApp());
}
