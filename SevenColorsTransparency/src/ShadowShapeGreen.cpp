#include "ShadowShapeGreen.h"

void ShadowShapeGreen::setup( float betweenThreshold ){
    ShadowShapeCore::setup(betweenThreshold);
    sankakuImage.loadImage("sankaku.png");
    
    imgW = sankakuImage.getWidth();
    imgH = sankakuImage.getHeight();
}

void ShadowShapeGreen::draw( float drawX, float drawY, float drawRatioW, float drawRatioH ){
    
    float time = ofGetElapsedTimeMillis();
    
    
    ofFill();
    ofSetColor(0,0,0,255*power);
    
    for( int i = 0, imax = shapeBlobs.size(); i < imax; ++i ) {
        vector<vector<float> > shapeBlob = shapeBlobs[i];
        for( int j = 0, jmax = shapeBlob.size(); j < jmax; ++j ) {
            vector<float> shape = shapeBlob[j];
            
            float n = shape[4];
            float s = shape[5];
            
            float x = shape[0] + shape[2] * 30;
            float y = shape[1] + shape[3] * 30;
            
            float dX = x * drawRatioW + drawX;
            float dY = y * drawRatioH + drawY;
            
            float deg = ofRadToDeg(atan2(shape[3], shape[2]));
            
            // ofCircle(dX, dY, n * 40);
            ofPushMatrix();
            ofTranslate(dX, dY);
            ofRotateZ(time * 0.1 + 360 * n);
            sankakuImage.draw(imgW * -0.5, imgH * -0.5);
            ofPopMatrix();
        }
    }
    
    ofSetColor(255);
    
}