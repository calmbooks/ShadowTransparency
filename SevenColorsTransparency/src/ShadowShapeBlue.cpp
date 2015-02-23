#include "ShadowShapeBlue.h"

void ShadowShapeBlue::draw( float drawX, float drawY, float drawRatioW, float drawRatioH ){
    
    ofFill();
    ofSetColor(0,0,0,255*power);
    
    for( int i = 0, imax = shapeBlobs.size(); i < imax; ++i ) {
        vector<vector<float> > shapeBlob = shapeBlobs[i];
        for( int j = 0, jmax = shapeBlob.size(); j < jmax; ++j ) {
            vector<float> shape = shapeBlob[j];
            
            float n = shape[4];
            float s = shape[5];
            
            float x = shape[0];
            float y = shape[1];
            
            float tX = x + shape[2] * n * 80 * s;
            float tY = y + shape[3] * n * 80 * s;
            
            float dX = x * drawRatioW + drawX;
            float dY = y * drawRatioH + drawY;
            float dtX = tX * drawRatioW + drawX;
            float dtY = tY * drawRatioH + drawY;
            
            ofCircle(dX, dY, n * 40);
        }
    }
    
    ofSetColor(255);
    
}