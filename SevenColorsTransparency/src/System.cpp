#include "System.h"

float System::mix( float p1, float p2, float ratio ) {
    return ( p2 - p1 ) * ratio + p1;
}

ofPoint System::mix( ofPoint p1, ofPoint p2, float ratio ){
    float x = ( p2.x - p1.x ) * ratio + p1.x;
    float y = ( p2.y - p1.y ) * ratio + p1.y;
    return ofVec2f(x,y);
}

ofVec2f System::mixVec( ofVec2f bVec, ofVec2f nVec, float ratio ){
    float bRad = atan2(bVec.y, bVec.x);
    float nRad = atan2(nVec.y, nVec.x);
    
    while( nRad - bRad > PI ) {
        nRad -= 360;
    }
    
    while( nRad - bRad < -PI ) {
        nRad += 360;
    }
    
    float rad = ( nRad - bRad ) * ratio + bRad;
    
    return ofVec2f(cos(rad), sin(rad));
}

ofColor System::hsv2rgb( float h, float s, float v ){
    
    int i = int(floor(h / 60)) % 6;
    float f = (h / 60) - floor(h / 60);
    float p = round(v * (1 - (s / 255)));
    float q = round(v * (1 - (s / 255) * f));
    float t = round(v * (1 - (s / 255) * (1 - f)));
    
    ofColor rgb;
    
    switch( i ) {
        case 0 : rgb = ofColor( v, t, p ); break;
        case 1 : rgb = ofColor( q, v, p ); break;
        case 2 : rgb = ofColor( p, v, t ); break;
        case 3 : rgb = ofColor( p, q, v ); break;
        case 4 : rgb = ofColor( t, p, v ); break;
        case 5 : rgb = ofColor( v, p, q );
    }
    
    return rgb;
}