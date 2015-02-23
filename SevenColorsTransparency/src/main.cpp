#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofSetupOpenGL(1280,800,OF_FULLSCREEN);
	ofRunApp(new ofApp());
    
    
    // ofSetupOpenGL(&win,1920*2,1080*2,OF_FULLSCREEN);			// <-------- setup the GL context
    //    ofSetupOpenGL(1920,1080,OF_FULLSCREEN);


    // ofHideCursor();
}
