#include "ShadowShapeCore.h"

void ShadowShapeCore::setup( float betweenThreshold ){
    BETWEEN_THRESHOLD = betweenThreshold;
    // ofSetCircleResolution(30);
}

void ShadowShapeCore::addBlob( ApproxBlob blob ){
    targetBlobs.push_back(blob);
}

vector<vector<vector<float> > > ShadowShapeCore::createShapeBlobs( vector<ApproxBlob> blobs ){
    
    vector<vector<vector<float> > > result;
    
    for( int i = 0, max = blobs.size(); i < max; ++i ) {
        result.push_back(createShapes(blobs[i]));
    }
    
    return result;
}

vector<vector<float> > ShadowShapeCore::createShapes( ApproxBlob blob ){
    
    vector<vector<float> > result;
    float totalLength = 0;
    
    for( int i = 1, max = blob.pts.size(); i < max; ++i ) {
        ofPoint lPt = blob.pts[i-1];
        ofPoint cPt = blob.pts[i];
        
        totalLength += ofVec2f(cPt.x-lPt.x, cPt.y-lPt.y).length();
    }
    
    float countLength = 0;
    
    int extend = 5;
    float threshold = BETWEEN_THRESHOLD;
    float scale = (totalLength * threshold) / 100;
    
    vector<vector<float> > beforeShapeBlob;
    
    if( blob.beforeFrameBlobIndex >= 0 && beforeShapeBlobs.size() > 0 ) {
        beforeShapeBlob = beforeShapeBlobs[blob.beforeFrameBlobIndex];
    }
    
    for( int i = 1, max = blob.pts.size(); i < max; ++i ) {
        ofPoint lPt = blob.pts[i-1];
        ofPoint cPt = blob.pts[i];
        
        ofVec2f cVec = ofVec2f(cPt.x-lPt.x, cPt.y-lPt.y);
        float cLen = cVec.length();
        
        countLength += cLen;
        
        float lengthRatio = countLength / totalLength;
        
        if( lengthRatio > threshold ) {
            // if( countLength > 50 ) {
            
            int bIndex = i - extend;
            int nIndex = i + extend - 1;
            
            if( bIndex < 0 ) bIndex = 0;
            if( nIndex >= max ) nIndex = max - 1;
            
            ofPoint bPt = blob.pts[bIndex];
            ofPoint nPt = blob.pts[nIndex];
            
            float backRatio = (lengthRatio - threshold) / (cLen / totalLength);
            
            ofPoint uPt = ofPoint(cPt.x - cVec.x * backRatio, cPt.y - cVec.y * backRatio);
            
            vector<float> shapes = createShape(bPt, uPt, nPt, result.size(), scale, beforeShapeBlob);
            result.push_back(shapes);
            countLength = 0;
        }
    }
    
    return result;
}

vector<float> ShadowShapeCore::createShape( ofPoint bPt, ofPoint uPt, ofPoint nPt, int index, float scale, vector<vector<float> > beforeShapeBlob ){
    
    vector<float> result;
    
    float noise = ofNoise(float(index) * 1234, float(index) * 1234);
    ofVec2f vVec = getVerticalVector(ofVec2f(nPt.x - bPt.x, nPt.y - bPt.y));
    
    if( beforeShapeBlob.size() > 0 ) {
        bool isFlag = false;
        const float maxLen = 100;
        const float tLen = 20;
        
        for( int i = 0, max = beforeShapeBlob.size(); i < max; ++i ) {
            vector<float> beforeShape = beforeShapeBlob[i];
            float len = ofVec2f(uPt.x - beforeShape[0], uPt.y - beforeShape[1]).length();
            
            if( len < maxLen && beforeShape[4] == noise ) {
                
                float ptRatio = len < tLen ? pow(len/maxLen, 2) : 1-pow((maxLen-len)/maxLen,2);
                ofPoint mPt = System::mix(ofPoint(beforeShape[0], beforeShape[1]), uPt, ptRatio);
                
                result.push_back(mPt.x);
                result.push_back(mPt.y);
                
                ofVec2f mVec = System::mixVec(ofVec2f(beforeShape[2], beforeShape[3]), vVec, 0.01);
                
                result.push_back(mVec.x);
                result.push_back(mVec.y);
                result.push_back(noise);
                result.push_back(scale);
                
                isFlag = true;
                break;
            }
        }
        if( isFlag ) {
            return result;
        }
    }
    
    result.push_back(uPt.x);
    result.push_back(uPt.y);
    result.push_back(vVec.x);
    result.push_back(vVec.y);
    result.push_back(noise);
    result.push_back(scale);
    
    // result.push_back((index * 2 % 20) * scale);
    
    return result;
}

ofVec2f ShadowShapeCore::getVerticalVector( ofVec2f vec ){
    
    ofVec2f result;
    float vecLength = 1;
    
    float n1 = pow(vecLength, 2);
    float n2 = n1 * pow(vec.y, 2);
    float n3 = pow(vec.x, 2) + pow(vec.y, 2);
    
    result.x = sqrt(n2 / n3);
    result.y = sqrt(n1 - pow(result.x, 2));
    
    if(vec.x <= 0 && vec.y < 0 ) { // 第3象限
        result.x *= -1;
    }
    else if( vec.x >= 0 && vec.y <= 0 ) { // 第4象限
        result.x *= -1;
        result.y *= -1;
    }
    else if( vec.x > 0 && vec.y >= 0 ) { // 第1象限
        result.y *= -1;
    }
    
    result.x *= -1;
    result.y *= -1;
    
    return result;
}

void ShadowShapeCore::update( float currentPower ){
    power = currentPower;
    shapeBlobs = createShapeBlobs(targetBlobs);
    targetBlobs.clear();
    
    beforeShapeBlobs = shapeBlobs;
}

void ShadowShapeCore::draw( float drawX, float drawY, float drawRatioW, float drawRatioH ){
    
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
            
            ofSetLineWidth(n * 15 * s);
            
            float dX = x * drawRatioW + drawX;
            float dY = y * drawRatioH + drawY;
            float dtX = tX * drawRatioW + drawX;
            float dtY = tY * drawRatioH + drawY;
            
            ofLine(dX, dY, dtX, dtY);
        }
    }
    
    ofSetLineWidth(1);
    ofSetColor(255);
    
    
    
    
    
    
    
    
    /*
     ofFill();
     ofSetHexColor(0xff0000);
     
     ofPath rline = ofPath();
     rline.setStrokeColor(ofColor(255,0,0,100));
     rline.setFilled(false);
     rline.setStrokeWidth(1);
     rline.moveTo(0, 0);
     
     for( float i = 0, imax = 1000; i <= imax; ++i ) {
     
     float x = i;
     float y = ofNoise(i,i) * 800;
     
     rline.lineTo(x, y);
     ofCircle(x, y, 2);
     }
     
     rline.draw();
     */
    
    
    
    
    
    /*
     ofCircle(100,100,100);
     ofEllipse(800,100,100,300);
     
     ofSetRectMode(OF_RECTMODE_CENTER);
     ofRect(200,200,300,200);
     ofSetRectMode(OF_RECTMODE_CORNER);
     
     ofSetHexColor(0x0000FF);
     ofTriangle(300,300,400,500,700,100);
     
     ofNoFill();
     ofSetLineWidth(10);
     ofSetHexColor(0x00FFFF);
     ofLine(600,600,650,800);
     ofBezier(600,200,700,500,900,500,1000,200);
     // ofSetHexColor(0xFF7700);
     // ofCurve(700,500,600,200,1000,200,900,500);
     ofSetLineWidth(1);
     ofFill();
     
     ofSetPolyMode(OF_POLY_WINDING_NONZERO);
     ofBeginShape();
     ofVertex(400,135);
     ofVertex(215,135);
     ofVertex(365,25);
     ofVertex(305,200);
     ofVertex(250,25);
     ofEndShape();
     
     ofNoFill();
     ofSetHexColor(0x0088FF);
     ofBeginShape();
     ofVertex(900,100);
     ofBezierVertex(900,500,1200,500,1200,100);
     ofVertex(1050,50);
     ofBezierVertex(950,0,950,0,900,100);
     ofEndShape();
     ofFill();
     
     ofPath line = ofPath();
     line.setStrokeColor(ofColor(255,0,0));
     line.setFillColor(ofColor(255,255,0));
     line.setStrokeWidth(3);
     line.moveTo(400,435);
     line.lineTo(215,435);
     line.lineTo(365,325);
     line.lineTo(305,500);
     line.lineTo(250,325);
     line.close();
     line.draw();
     
     ofPath bline = ofPath();
     bline.setStrokeColor(ofColor(255,0,0));
     bline.setFillColor(ofColor(255,255,0));
     bline.setStrokeWidth(3);
     bline.moveTo(900,100);
     bline.bezierTo(900,500,1200,500,1200,100);
     bline.lineTo(1050,50);
     bline.bezierTo(950,0,950,0,900,100);
     bline.close();
     bline.draw();
     */
}
