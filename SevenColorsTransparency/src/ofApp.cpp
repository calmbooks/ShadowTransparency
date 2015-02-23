#include "ofApp.h"

void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    windowW = ofGetWidth();
    windowH = ofGetHeight();
    
    buffer.allocate(windowW, windowH);
    
    SPOT_COLORS.push_back(ofColor(255,255,200));
    SPOT_COLORS.push_back(ofColor(255,200,200));
    SPOT_COLORS.push_back(ofColor(200,255,200));
    SPOT_COLORS.push_back(ofColor(200,225,255));
    
    spotColorPowers.push_back(1);
    spotColorPowers.push_back(0);
    spotColorPowers.push_back(0);
    spotColorPowers.push_back(0);
    
    targetSpotColorN = 1;
        
    spotLightImage.loadImage("spotLight.png");
    
#ifdef USE_KINECT
    kinect.setRegistration(true);
    kinect.init();
    // kinect.init(false, false);
    kinect.open();
    imageWidth = kinect.width;
    imageHeight = kinect.height;
    threshold = 204;
   	angle = 4;
    kinect.setCameraTiltAngle(angle);
#else
    testImage.loadImage("test_shapes2.png");
    imageWidth = testImage.getWidth();
    imageHeight = testImage.getHeight();
    threshold = 230;
#endif
    
    grayImage.allocate(imageWidth, imageHeight);
    grayImage2.allocate(imageWidth, imageHeight);
    
    ofSetFrameRate(60);
    
    shadowShapeRed.setup(0.01);
    shadowShapeGreen.setup(0.05);
    shadowShapeBlue.setup(0.03);
    
    drawOriginX = 0.55;
    drawOriginY = 0.44;
    
    drawRatioW = 1.31;
    drawRatioH = 1.42;
    
    roiX = 159;
    roiY = 117;
    roiW = 223;
    roiH = 202;
}

void ofApp::update(){
    
    ofBackground(0);
    
    drawImageW = roiW * 3;
    drawImageH = roiH * 3;
    
#ifdef USE_KINECT
    kinect.update();
    isKinectFrameUpdate = kinect.isFrameNew();
#else
    isKinectFrameUpdate = true;
#endif
    
    if( isKinectFrameUpdate ) {
        
#ifdef USE_KINECT
        grayImage.setFromPixels(kinect.getDepthPixels(), imageWidth, imageHeight);
        grayImage.setROI(roiX,roiY,roiW,roiH);
        grayImage2.setROI(roiX,roiY,roiW,roiH);
        grayImage.resize(drawImageW,drawImageH);
        grayImage2.resize(drawImageW,drawImageH);
#else
        colorImg.setFromPixels(testImage.getPixels(), imageWidth, imageHeight);
        grayImage = colorImg;
#endif
        grayImage2 = grayImage;
        
        grayImage.threshold(threshold, false);
        grayImage2.threshold(threshold, true);
        
        contourFinder.findContours(grayImage, 500, imageWidth * imageHeight, 30, true, false);
    }
    
    isShapeChange = true;
    
    if( contourFinder.blobs.size() > 0 ) {
        approxBlobs = createApproxBlobs(contourFinder.blobs);
    
        if( isShapeChange ) {
            int before = targetSpotColorN;
            targetSpotColorN = int(ofRandom(1,4));
            while( before == targetSpotColorN ) {
                targetSpotColorN = int(ofRandom(1,4));
            }
        }
    }
    else {
        targetSpotColorN = 0;
    }
    
    
    updateSpotColor();
    
    if( approxBlobs.size() > 0 ) {
        
        for( int i = 0, imax = approxBlobs.size(); i < imax; ++i ) {
            ApproxBlob blob = approxBlobs[i];
            if( spotColorPowers[1] > THRESHOLD_POWER ) {
                shadowShapeRed.addBlob(blob);
            }
            if( spotColorPowers[2] > THRESHOLD_POWER ) {
                shadowShapeGreen.addBlob(blob);
            }
            if( spotColorPowers[3] > THRESHOLD_POWER ) {
                shadowShapeBlue.addBlob(blob);
            }
        }
        
        if( spotColorPowers[1] > THRESHOLD_POWER ) {
            shadowShapeRed.update(spotColorPowers[1]);
        }
        if( spotColorPowers[2] > THRESHOLD_POWER ) {
            shadowShapeGreen.update(spotColorPowers[2]);
        }
        if( spotColorPowers[3] > THRESHOLD_POWER ) {
            shadowShapeBlue.update(spotColorPowers[3]);
        }
    }
    
    beforeApproxBlobs = approxBlobs;
    
}

vector<ApproxBlob> ofApp::createApproxBlobs( vector<ofxCvBlob> blobs ){
    vector<ApproxBlob> result;
   
    const float THRESHOLD_AREA_DIFF = 5000;
    const float THRESHOLD_SIZE_DIFF = 300;
    
    
    for( int i = 0, imax = blobs.size(); i < imax; ++i ) {
        ofxCvBlob blob = blobs[i];
        ApproxBlob approxBlob = ApproxBlob(blob.pts, blob.area);
        
        approxBlob.beforeFrameBlobIndex = -1;
        
        for( int j = 0, jmax = beforeApproxBlobs.size(); j < jmax; ++j ) {
            ApproxBlob beforeBlob = beforeApproxBlobs[j];
            float areaDiff = abs(beforeBlob.area - blob.area);
            float sizeDiff = abs(float(beforeBlob.bPts.size()) - float(blob.nPts));
            if( areaDiff < THRESHOLD_AREA_DIFF && sizeDiff < THRESHOLD_SIZE_DIFF ) {
                approxBlob.beforeFrameBlobIndex = j;
                isShapeChange = false;
                break;
            }
        }
        
        result.push_back(approxBlob);
    }
    
    return result;
}

void ofApp::updateSpotColor(){
    
    ofColor nColor = ofColor(0,0,0);

    for( int i = 0, max = spotColorPowers.size(); i < max; ++i ) {
        float cPower = spotColorPowers[i];
        if( i == targetSpotColorN ) {
            spotColorPowers[i] = System::mix(cPower,1,0.1);
        }
        else {
            spotColorPowers[i] = System::mix(cPower,0,0.1);
        }
        
        nColor.r += SPOT_COLORS[i].r * spotColorPowers[i];
        nColor.g += SPOT_COLORS[i].g * spotColorPowers[i];
        nColor.b += SPOT_COLORS[i].b * spotColorPowers[i];
    }
    
    currentColor = nColor;
}

void ofApp::draw(){
    
    buffer.begin();
    
    ofSetColor(255);
    
    drawSpotLight();
    
    float drawKinectW = drawImageW * drawRatioW;
    float drawKinectH = drawImageH * drawRatioH;
    float drawKinectX = windowW * 0.5 - drawKinectW * drawOriginX;
    float drawKinectY = windowH * 0.5 - drawKinectH * drawOriginY;
    
#ifdef USE_KINECT
    // grayImage2.drawROI(drawKinectX, drawKinectY, drawKinectW, drawKinectH);
    // contourFinder.draw(drawKinectX, drawKinectY, drawKinectW, drawKinectH);
#else
    grayImage2.draw(0,0);
    contourFinder.draw();
#endif
    
    // drawFirstPoint(drawKinectX, drawKinectY, drawRatioW, drawRatioH);
    
    if( spotColorPowers[1] > THRESHOLD_POWER ) {
        shadowShapeRed.draw(drawKinectX, drawKinectY, drawRatioW, drawRatioH);
    }
    
    if( spotColorPowers[2] > THRESHOLD_POWER ) {
        shadowShapeGreen.draw(drawKinectX, drawKinectY, drawRatioW, drawRatioH);
    }
    
    if( spotColorPowers[3] > THRESHOLD_POWER ) {
        shadowShapeBlue.draw(drawKinectX, drawKinectY, drawRatioW, drawRatioH);
    }
    
    /*
    for( int i = 0, imax = approxBlobs.size(); i < imax; ++i ) {
        approxBlobs[i].draw(drawKinectX, drawKinectY, drawRatioW, drawRatioH);
    }
     */
    
    // drawRoiArea();
    // drawHoleString();
    // drawReport();
    
    buffer.end();
    
    ofSetColor(255);
    
    // ofPixels pixels;
    // pixels.allocate(windowW, windowH, OF_PIXELS_RGB);
    // buffer.readToPixels(pixels);
    // bufferImage.setFromPixels(pixels);
    
    buffer.draw(0,0);
    
}

void ofApp::drawSpotLight(){
    // ofColor rgb = System::hsv2rgb(200,20,255);
    ofSetColor(currentColor);
    spotLightImage.draw(0,0);
    /*
    ofFill();
    ofSetHexColor(0xffffff);
    ofSetLineWidth(10);
    ofCircle(windowW * 0.5, windowH * 0.5, windowH * 0.5);
     */
}

void ofApp::drawFirstPoint( float drawX, float drawY, float drawRatioW, float drawRatioH ){
    
    ofFill();
    ofSetHexColor(0xff0000);
    for( int i = 0, imax = contourFinder.nBlobs; i < imax; ++i ) {
        ofPoint firstPt = contourFinder.blobs[i].pts[0];
        float x = firstPt.x * drawRatioW + drawX;
        float y = firstPt.y * drawRatioH + drawY;
        ofCircle(x, y, 10);
    }
}

void ofApp::drawRoiArea(){
    ofSetColor(255);
    kinect.draw(0,0);
    ofSetColor(255,0,0);
    ofNoFill();
    ofSetLineWidth(3);
    ofRect(roiX,roiY,roiW,roiH);
}

void ofApp::drawHoleString(){
    
    ofSetHexColor(0xFFFF00);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        if( contourFinder.blobs[i].hole ) {
            ofPoint center = contourFinder.blobs[i].boundingRect.getCenter();
            ofDrawBitmapString("hole", center.x, center.y);
        }
    }
}

void ofApp::drawReport(){
    
    stringstream reportStr;
    
    reportStr << "threshold " << threshold << endl
    << "blobs found " << contourFinder.nBlobs << endl
    << "drawOriginX " << drawOriginX << ", drawOriginY " << drawOriginY << ", drawRatioW " << drawRatioW << ", drawRatioH " << drawRatioH << endl
    << "roiX " << roiX << ", roiY " << roiY << ", roiW " << roiW << ", roiH " << roiH << endl
    << "FPS " << ofGetFrameRate();
    
    ofSetHexColor(0xFF0000);
    ofDrawBitmapString(reportStr.str(), 20, 20);
}

void ofApp::exit(){
#ifdef USE_KINECT
    kinect.close();
#endif
}

void ofApp::keyPressed(int key){
    
    switch (key){
            
        case '0':
            targetSpotColorN = 0;
            break;
            
        case '1':
            targetSpotColorN = 1;
            break;
            
        case '2':
            targetSpotColorN = 2;
            break;
            
        case '3':
            targetSpotColorN = 3;
            break;
            
        case 't':
            threshold ++;
            break;
            
        case 'T':
            threshold --;
            break;
            
        case 'x':
            drawOriginX += 0.01;
            break;
            
        case 'X':
            drawOriginX -= 0.01;
            break;
            
        case 'y':
            drawOriginY += 0.01;
            break;
            
        case 'Y':
            drawOriginY -= 0.01;
            break;
            
        case 'w':
            drawRatioW += 0.01;
            break;
            
        case 'W':
            drawRatioW -= 0.01;
            break;
    
        case 'h':
            drawRatioH += 0.01;
            break;
            
        case 'H':
            drawRatioH -= 0.01;
            break;
            
        case 'a':
            roiX += 1;
            break;
            
        case 'A':
            roiX -= 1;
            break;
            
        case 's':
            roiY += 1;
            break;
            
        case 'S':
            roiY -= 1;
            break;
            
        case 'd':
            roiW += 1;
            break;
            
        case 'D':
            roiW -= 1;
            break;
            
        case 'f':
            roiH += 1;
            break;
            
        case 'F':
            roiH -= 1;
            break;
    }
}



