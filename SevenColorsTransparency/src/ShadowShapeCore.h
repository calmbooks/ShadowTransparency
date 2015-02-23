#pragma once

#include "ofMain.h"
#include "System.h"
#include "ofxOpenCv.h"
#include "ApproxBlob.h"

class ShadowShapeCore {
    
public:
    void setup( float betweenThreshold );
    void update( float currentPower );
    void draw( float drawX, float drawY, float drawRatioW, float drawRatioH );
    
    void addBlob( ApproxBlob blob );
    
    vector<vector<vector<float> > > createShapeBlobs( vector<ApproxBlob> blobs );
    vector<vector<float> > createShapes( ApproxBlob blob );
    vector<float> createShape( ofPoint bPt, ofPoint uPt, ofPoint nPt, int index, float scale, vector<vector<float> > beforeShapeBlob );
    ofVec2f getVerticalVector( ofVec2f vec );
    
    vector<ApproxBlob> targetBlobs;
    
    vector<vector<vector<float> > > shapeBlobs;
    vector<vector<vector<float> > > beforeShapeBlobs;
    
    float power;
    
    float BETWEEN_THRESHOLD;
};