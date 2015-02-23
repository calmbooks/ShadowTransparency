#pragma once

#include "ofMain.h"

class System {
public:
    static float mix( float p1, float p2, float ratio );
    static ofPoint mix( ofPoint p1, ofPoint p2, float ratio );
    static ofVec2f mixVec( ofVec2f bVec, ofVec2f nVec, float ratio );
    static ofColor hsv2rgb( float h, float s, float v );
};

