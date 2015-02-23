#pragma once

#include "ofMain.h"

class ApproxBlob {
public:
    ApproxBlob( vector<ofPoint> points, float blobArea );
    ~ApproxBlob();
    
    vector<ofPoint> smoothFlatPoints( vector<ofPoint> points );
    vector<float> getDegs( vector<ofPoint> points );
    vector<float> removalExtremeNumbers( vector<float> numbers );
    vector<float> smoothFlatNumbers( vector<float> numbers );
    
    vector<ofPoint> createApproxPoints( vector<float> degs );
    
    void draw( float drawX, float drawY, float drawRatioW, float drawRatioH );
    void drawBeforePoints( float drawX, float drawY, float drawRatioW, float drawRatioH );
    void drawPoints( float drawX, float drawY, float drawRatioW, float drawRatioH );
    void drawLine( float drawX, float drawY, float drawRatioW, float drawRatioH );
    void drawDebug();
    
    int beforeFrameBlobIndex;
    float area;
    
    vector<ofPoint> bPts;
    vector<ofPoint> sPts;
    vector<ofPoint> pts;
    
    vector<float> degs;
    vector<float> rDegs;
    vector<float> sDegs;
    
    vector<int> approxIndexes;
    
    int THRESHOLD_REMOVAL_EXTREME_NUMBERS_RANGE = 20;
    float THRESHOLD_REMOVAL_EXTREME_NUMBERS_DIFF = 10;
    
    int THRESHOLD_SMOOTH_FLAT_NUMBERS_RANGE = 20;
};