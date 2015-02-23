#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "ShadowShapeRed.h"
#include "ShadowShapeBlue.h"
#include "ShadowShapeGreen.h"
#include "ApproxBlob.h"

#define USE_KINECT

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();

    void updateSpotColor();
    
    void drawSpotLight();
    void drawFirstPoint( float drawX, float drawY, float drawRatioW, float drawRatioH );
    void drawRoiArea();
    void drawHoleString();
    void drawReport();
    void keyPressed( int key );
    vector<ApproxBlob> createApproxBlobs( vector<ofxCvBlob> blobs );
    
#ifdef USE_KINECT
    ofxKinect kinect;
#else
    ofImage testImage;
    ofxCvColorImage colorImg;
#endif
    
    ofImage spotLightImage;
    
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayImage2;
    
    ofxCvGrayscaleImage bufferImage;
    
    ofxCvContourFinder contourFinder;
    
    vector<ApproxBlob> beforeApproxBlobs;
    vector<ApproxBlob> approxBlobs;
    
    ShadowShapeRed shadowShapeRed;
    ShadowShapeGreen shadowShapeGreen;
    ShadowShapeBlue shadowShapeBlue;
    
    int threshold;
    int angle;
    
    int imageWidth;
    int imageHeight;
    
    int windowW;
    int windowH;
    
    float drawImageW;
    float drawImageH;
    
    float drawRatioW;
    float drawRatioH;
    float drawOriginX;
    float drawOriginY;
    
    float roiW;
    float roiH;
    float roiX;
    float roiY;
    
    int targetSpotColorN;
    
    vector<ofColor> SPOT_COLORS;
    vector<float> spotColorPowers;
    ofColor currentColor;
    
    bool isKinectFrameUpdate;
    bool isShapeChange;
    
    const float THRESHOLD_POWER = 0.01;
    
    ofFbo buffer;
};
