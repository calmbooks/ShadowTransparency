#include "ApproxBlob.h"

ApproxBlob::ApproxBlob( vector<ofPoint> points, float blobArea ) {
    
    area = blobArea;
    
    bPts = points;
    // sPts = smoothFlatPoints(bPts);
    
    degs = getDegs(bPts);
    rDegs = removalExtremeNumbers(degs);
    sDegs = smoothFlatNumbers(rDegs);
    
    pts = createApproxPoints(sDegs);
    pts = bPts;
    
    return;
    
    // cout << "--------" << endl;
    
    /*
    pts.push_back(points[0]);
  
    for( int i = 1, imax = points.size() - 1; i < imax; ++i ) {
        ofPoint bPoint = points[i - 1];
        ofPoint cPoint = points[i];
        ofPoint nPoint = points[i + 1];
        
        ofVec2f bVec = ofVec2f(cPoint.x - bPoint.x, cPoint.y - bPoint.y);
        float bDeg = ofRadToDeg(atan2( bVec.y, bVec.x));
        bDeg = bDeg < 0 ? (bDeg + 360) : bDeg;
        
        ofVec2f nVec = ofVec2f(nPoint.x - cPoint.x, nPoint.y - cPoint.y);
        float nDeg = ofRadToDeg(atan2( nVec.y, nVec.x));
        nDeg = nDeg < 0 ? (nDeg + 360) : nDeg;
        
        cout << bDeg << endl;
        
        if( abs(bDeg - nDeg) > 50 ) {
            pts.push_back(cPoint);
        }
    }
    */
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*
    // cout << "--------" << endl;
    
    int pointLength = points.size();
    
    int cIndex = 0;
    int tIndex = 1;
    
    ofPoint cPoint;
    ofPoint tPoint;
    
    ofVec2f cVec;
    
    const float THRESHOLD = 25;
    
    float cDeg;
    float minDeg;
    float maxDeg;
    
    pts.push_back(points[cIndex]);
    
    while( cIndex < pointLength - 1 ) {
        // cout << "while---" << endl;
        cPoint = points[cIndex];
        
        for( tIndex = cIndex + 1; tIndex < pointLength; ++tIndex ) {
            tPoint = points[tIndex];
            
            cVec = ofVec2f(tPoint.x - cPoint.x, tPoint.y - cPoint.y);
            cDeg = ofRadToDeg(atan2(cVec.y, cVec.x));
            
            cDeg = cDeg < 0 ? (cDeg + 360) : cDeg;
            
            if( tIndex == cIndex + 1 ) {
                minDeg = cDeg;
                maxDeg = cDeg;
            }
            
            // cout << "---"<< endl;
            // cout << cDeg << endl;
            // cout << minDeg << endl;
            // cout << maxDeg << endl;
            
            if( cDeg - minDeg > THRESHOLD || maxDeg - cDeg > THRESHOLD ) {
                // cout << "true" << endl;
                pts.push_back(points[tIndex - 1]);
                cIndex = tIndex;
                break;
            }
            else if( tIndex == pointLength - 1 ) {
                cIndex = tIndex;
            }
            
            minDeg = min(minDeg, cDeg);
            maxDeg = max(maxDeg, cDeg);
        }
    }
     */
}

ApproxBlob::~ApproxBlob() {
}

vector<ofPoint> ApproxBlob::smoothFlatPoints( vector<ofPoint> points ) {
    
    vector<ofPoint> result;
    
    result.push_back(points[0]);
   
    for( int i = 1, imax = points.size() - 1; i < imax; ++i ) {
        ofPoint bPoint = points[i - 1];
        ofPoint cPoint = points[i];
        ofPoint nPoint = points[i + 1];
        
        ofVec2f bVec = ofVec2f(cPoint.x - bPoint.x, cPoint.y - bPoint.y);
        ofVec2f nVec = ofVec2f(nPoint.x - cPoint.x, nPoint.y - cPoint.y);
        ofVec2f sVec = ofVec2f(nPoint.x - bPoint.x, nPoint.y - bPoint.y);
        
        float bDeg = ofRadToDeg(atan2(bVec.y, bVec.x));
        float nDeg = ofRadToDeg(atan2(nVec.y, nVec.x));
        
        while( nDeg - bDeg > 180 ) {
            nDeg -= 360;
        }
        
        while( nDeg - bDeg < -180 ) {
            nDeg += 360;
        }
        
        if( bDeg >= nDeg ) {
            result.push_back(cPoint);
        }
        else {
            result.push_back(ofPoint(bPoint.x + sVec.x * 0.5, bPoint.y + sVec.y * 0.5 ));
        }
    }
    
    result.push_back(points.back());
    
    return result;
}

vector<float> ApproxBlob::getDegs( vector<ofPoint> points ) {
    
    vector<float> degs;
    float bDeg = 0;
    
    for( int i = 0, imax = points.size(); i < (imax - 1); ++i ) {
        ofPoint cPoint = points[i];
        ofPoint nPoint = points[i + 1];
        
        ofVec2f nVec = ofVec2f(nPoint.x - cPoint.x, nPoint.y - cPoint.y);
        
        float nDeg = ofRadToDeg(atan2(nVec.y, nVec.x));
        
        while( nDeg - bDeg > 180 ) {
            nDeg -= 360;
        }
        
        while( nDeg - bDeg < -180 ) {
            nDeg += 360;
        }
        
        degs.push_back(nDeg);
        bDeg = nDeg;
    }
    
    return degs;
}

vector<float> ApproxBlob::removalExtremeNumbers( vector<float> numbers ) {
    
    vector<float> result;
    
    const int THRESHOLD_RANGE = THRESHOLD_REMOVAL_EXTREME_NUMBERS_RANGE;
    const float THRESHOLD_DIFF = THRESHOLD_REMOVAL_EXTREME_NUMBERS_DIFF;
    
    for( int i = 0, imax = numbers.size(); i < imax; ++i ) {
        float cNum = numbers[i];
        vector<float> stack;
        
        int j = i - THRESHOLD_RANGE * 0.5;
        int jmax = i + THRESHOLD_RANGE * 0.5;
        
        if( j < 0 ) {
            j = 0;
            jmax = j + THRESHOLD_RANGE;
        }
        else if( j >= imax ) {
            jmax = imax;
            j = jmax - THRESHOLD_RANGE;
        }
        
        while(j <= jmax) {
            stack.push_back(numbers[j]);
            j += 1;
        }
        
        sort(stack.begin(), stack.end());
        
        if( stack[0] == cNum && abs(stack[1] - stack[0]) > THRESHOLD_DIFF ) {
            result.push_back(stack[1]);
        }
        else if( stack.back() == cNum && abs(stack.back() - stack[stack.size()-2]) > THRESHOLD_DIFF  ) {
            result.push_back(stack[stack.size()-2]);
        }
        else {
            result.push_back(cNum);
        }
    }
    
    return result;
}

vector<float> ApproxBlob::smoothFlatNumbers( vector<float> numbers ) {
    
    vector<float> result;
 
    const int THRESHOLD_RANGE = THRESHOLD_SMOOTH_FLAT_NUMBERS_RANGE;
   
    for( int i = 0, imax = numbers.size(); i < imax; ++i ) {
        float cNum = numbers[i];
        vector<float> stack;
        
        int j = i - THRESHOLD_RANGE * 0.5;
        int jmax = i + THRESHOLD_RANGE * 0.5;
        
        if( j < 0 ) {
            j = 0;
            jmax = j + THRESHOLD_RANGE;
        }
        else if( jmax >= imax ) {
            jmax = imax - 1;
            j = jmax - THRESHOLD_RANGE;
        }
        
        while(j <= jmax) {
            stack.push_back(numbers[j]);
            j += 1;
        }
        
        sort(stack.begin(), stack.end());
        result.push_back((stack[0] + stack[THRESHOLD_RANGE]) / 2);
    }
    
    return result;
}

vector<ofPoint> ApproxBlob::createApproxPoints( vector<float> degs ) {
    
    vector<ofPoint> result;
    
    float bDeg;
    float cDeg;
    
    const int THRESHOLD_DEG = 5;
    
    int bIndex;
    
    for( int i = 1, imax = degs.size(); i < imax; ++i ) {
        bDeg = degs[i - 1];
        cDeg = degs[i];
        
        if( abs(cDeg - bDeg) > THRESHOLD_DEG ) {
            int cIndex = i;
            if(approxIndexes.size() > 0) {
                int diff = cIndex - bIndex;
                if( diff < 11 ) {
                    if( approxIndexes.back() == bIndex ) approxIndexes.pop_back();
                    approxIndexes.push_back(bIndex + diff / 2);
                }
                else {
                    approxIndexes.push_back(cIndex);
                }
            }
            else {
                approxIndexes.push_back(cIndex);
            }
            bIndex = cIndex;
        }
    }
    
    result.push_back(bPts[0]);
    
    for( int i = 0, imax = approxIndexes.size(); i < imax; ++i ) {
        result.push_back(bPts[approxIndexes[i]]);
    }
    
    return result;
}

void ApproxBlob::draw( float drawX, float drawY, float drawRatioW, float drawRatioH ) {
    
    if( beforeFrameBlobIndex >= 0 ) {
        ofSetHexColor(0xff0000);
    }
    else {
        ofSetHexColor(0xffff00);
    }
    ofFill();
    ofCircle(1000,100,10);
    
    // drawBeforePoints(drawX, drawY, drawRatioW, drawRatioH);
    drawPoints(drawX, drawY, drawRatioW, drawRatioH);
    drawLine(drawX, drawY, drawRatioW, drawRatioH);
    // drawDebug();
}

void ApproxBlob::drawDebug() {
    
    const float ratioX = 3;
    const float ratioY = 1;
    const float originX = 6500;
    const float originY = -400;
    
    ofFill();
    ofSetHexColor(0xff0000);
    
    ofPath rline = ofPath();
    rline.setStrokeColor(ofColor(255,0,0,100));
    rline.setFilled(false);
    rline.setStrokeWidth(1);
    rline.moveTo(0, 0);
    
    for( int i = 5, imax = degs.size(); i < (imax - 5); ++i ) {
        
        float cDeg = degs[i];
        
        rline.lineTo(i * ratioX - originX, cDeg * ratioY - originY);
        ofCircle(i * ratioX - originX, cDeg * ratioY - originY, 2);
    }
    
    rline.draw();
    
    ofFill();
    ofSetHexColor(0x00ff00);
    
    ofPath gline = ofPath();
    gline.setStrokeColor(ofColor(0,255,0));
    gline.setFilled(false);
    gline.setStrokeWidth(1);
    gline.moveTo(0, 0);
    
    for( int i = 5, imax = rDegs.size(); i < (imax - 5); ++i ) {
        
        float cDeg = rDegs[i];
        
        gline.lineTo(i * ratioX - originX, cDeg * ratioY - originY);
        ofCircle(i * ratioX - originX, cDeg * ratioY - originY, 2);
    }
    
    gline.draw();
    
    ofFill();
    ofSetHexColor(0x0000ff);
    
    ofPath bline = ofPath();
    bline.setStrokeColor(ofColor(0,0,255));
    bline.setFilled(false);
    bline.setStrokeWidth(1);
    bline.moveTo(0, 0);
    
    for( int i = 5, imax = sDegs.size(); i < (imax - 5); ++i ) {
        
        float cDeg = sDegs[i];
        
        bline.lineTo(i * ratioX - originX, cDeg * ratioY - originY);
        ofCircle(i * ratioX - originX, cDeg * ratioY - originY, 2);
    }
    
    bline.draw();
    
    ofFill();
    ofSetHexColor(0xffff00);
    
    for( int i = 0, imax = approxIndexes.size(); i < imax; ++i ) {
        ofCircle(approxIndexes[i] * ratioX - originX, sDegs[approxIndexes[i]] * ratioY - originY, 6);
    }
}

void ApproxBlob::drawBeforePoints( float drawX, float drawY, float drawRatioW, float drawRatioH ) {
    ofFill();
    ofSetHexColor(0xffff00);
    for( int i = 0, imax = bPts.size(); i < imax; ++i ) {
        ofPoint pt = bPts[i];
        ofCircle(pt.x, pt.y, 2);
    }
}

void ApproxBlob::drawPoints( float drawX, float drawY, float drawRatioW, float drawRatioH ) {
    ofFill();
    ofSetHexColor(0xffff00);
    for( int i = 0, imax = pts.size(); i < imax; ++i ) {
        ofPoint pt = pts[i];
        float x = pt.x * drawRatioW + drawX;
        float y = pt.y * drawRatioH + drawY;
        ofCircle(x, y, 3);
    }
}

void ApproxBlob::drawLine( float drawX, float drawY, float drawRatioW, float drawRatioH ) {
    
    ofPath line = ofPath();
    line.setStrokeColor(ofColor(255,255,0));
    line.setFilled(false);
    line.setStrokeWidth(1);
    for( int i = 0, imax = pts.size(); i < imax; ++i ) {
        ofPoint pt = pts[i];
        float x = pt.x * drawRatioW + drawX;
        float y = pt.y * drawRatioH + drawY;
        
        if( i == 0 ) {
            line.moveTo(x, y);
        }
        else {
            line.lineTo(x, y);
        }
    }
    line.close();
    line.draw();
}



