#include "ShadowShapeRed.h"

void ShadowShapeRed::draw( float drawX, float drawY, float drawRatioW, float drawRatioH ){
    
    ofFill();
    ofSetColor(0,0,0,255*power);
    
    for( int i = 0, imax = shapeBlobs.size(); i < imax; ++i ) {
        vector<vector<float> > shapeBlob = shapeBlobs[i];
        for( int j = 0, jmax = shapeBlob.size(); j < jmax; ++j ) {
            vector<float> shape = shapeBlob[j];
            
            float n = shape[4];
            float s = shape[5];
            
            float x = shape[0] + shape[2] * 15;
            float y = shape[1] + shape[3] * 15;
            
            float tX = x + shape[2] * 20;
            float tY = y + shape[3] * 20;
            
            ofSetLineWidth(5);
            
            float dX = x * drawRatioW + drawX;
            float dY = y * drawRatioH + drawY;
            float dtX = tX * drawRatioW + drawX;
            float dtY = tY * drawRatioH + drawY;
            
            ofLine(dX, dY, dtX, dtY);
        }
    }
    
    ofSetLineWidth(1);
    ofSetColor(255);
    

}

