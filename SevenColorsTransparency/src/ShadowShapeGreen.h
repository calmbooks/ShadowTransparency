#pragma once

#include "ShadowShapeCore.h"

class ShadowShapeGreen  : public ShadowShapeCore{
public:
    void setup( float betweenThreshold );
    void draw( float drawX, float drawY, float drawRatioW, float drawRatioH );
    
    ofImage sankakuImage;
    float imgW;
    float imgH;
};