#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	#ifdef TARGET_OSX
		string newRoot = "../Resources/data/";
		ofEnableDataPath();
		ofSetDataPathRoot(newRoot);
	#endif
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}