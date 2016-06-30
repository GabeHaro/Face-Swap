#include "ofApp.h"

using namespace ofxDLib;

//--------------------------------------------------------------
void FaceAugmented::setup(const Face & track) {
    
    color.setHsb(ofRandom(0, 255), 255, 255);
    cur = track.rect.getCenter();
    smooth = cur;
    roi = track.rect;
    face = track;

}

//--------------------------------------------------------------
void FaceAugmented::update(const Face & track) {
    
    cur = track.rect.getCenter();
    roi = track.rect;
    smooth.interpolate(cur, .5);
    all.addVertex(smooth);
    face = track;
    
}

//--------------------------------------------------------------
void FaceAugmented::setImage(const ofPixels & pixels) {
    
    pixels.cropTo(image.getPixels(), roi.getX(), roi.getY(), roi.getWidth(), roi.getHeight());
    image.update();
    
}

//--------------------------------------------------------------
void FaceAugmented::kill() {
    
    dead = true;
    
}

//--------------------------------------------------------------
void FaceAugmented::draw() {
    
    ofPushStyle();
    ofSetColor(255);
    image.draw(roi);
    ofSetColor(255);
    ofNoFill();
    ofDrawCircle(cur, 16);
    ofSetColor(color);
    all.draw();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(label), cur);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Camera will be grabbed at this size
    camWidth = 1980;
    camHeight = 1080;
    
    //Initialize Cloning and Fbo objects
    cloneReady = false;
    clone1.setup(camWidth, camHeight);
    clone2.setup(camWidth, camHeight);
    ofFbo::Settings settings;
    settings.width = camWidth;
    settings.height = camHeight;
    srcFbo1.allocate(settings);
    srcFbo2.allocate(settings);
    maskFbo1.allocate(settings);
    maskFbo2.allocate(settings);
    
    //Get devices
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    //Select first device (probably webcam)
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    //Set up face tracker
    ft.setup("shape_predictor_68_face_landmarks.dat");
    ft.setSmoothingRate(0.5);
    ft.setDrawStyle(ofxDLib::lines);
    ofSetVerticalSync(true);

}

//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255, 255);
    
    //At each frame, save frame to image and pass this image to multitracker
    if(vidGrabber.isFrameNew()){
        ofPixels vidPixels = vidGrabber.getPixels();
        //vidPixels.rotate90(1);
        vidPixels.mirror(false, true);
        vidImg.setFromPixels(vidPixels);
        vidImg.draw(0, 0);
        
        vidPixels.resize(camWidth/SCALING, camHeight/SCALING);
        ft.findFaces(vidPixels, SCALING);
        vidPixels = vidImg.getPixels();
        tracker.track(ft.getFaces());
        vector<FaceAugmented>& facesAugmented = tracker.getFollowers();
        for (auto & face : facesAugmented) {
            face.setImage(vidPixels);
        }
    }
    
    //Displays last collected image between frames (avoids flickering)
    else
    vidImg.draw(0,0);
    
    ofSetColor(255, 255, 255);
    
    //Draw face features if only one face available
    ft.draw();
    
    //If 2 faces available conduct swap
    if(ft.size()>=2)
    {
        
        bool faceOn1 = false;
        vector<ofVec3f> landmarks1, landmarks2;
        
        //Collect shapes from dlib face tracker
        int c = 0;
        for (auto & face : ft.getFaces()) {
            if (ft.size() > 0 && c == 0) {
                landmarks1     =  face.landmarks;
            }
            if (ft.size() > 1 && c == 1) {
                landmarks2     =  face.landmarks;
            }
            c++;
        }
        
        //Get image points and make mesh
        vector<ofVec2f> facePoints1;
        vector<ofVec2f> facePoints2;
    
        faceMesh1.clear();
        faceMesh2.clear();
        
        faceMesh1.addVertices(landmarks1);
        faceMesh2.addVertices(landmarks2);
        
        faceMesh1.setMode(OF_PRIMITIVE_TRIANGLES);
        faceMesh2.setMode(OF_PRIMITIVE_TRIANGLES);
        
        faceMesh1.clearIndices();
        faceMesh2.clearIndices();
        
        //Custom index map for face alignment
        
        ////////////////////////////////////
        
        faceMesh1.addIndices({0,17,36});
        faceMesh1.addIndices({17,36,37});
        faceMesh1.addIndices({17,18,37});
        faceMesh1.addIndices({18,37,38});
        faceMesh1.addIndices({18,38,19});
        faceMesh1.addIndices({19,38,20});
        faceMesh1.addIndices({20,38,21});
        faceMesh1.addIndices({21,38,39});
        faceMesh1.addIndices({21,39,27});
        faceMesh1.addIndices({21,22,27});
        faceMesh1.addIndices({22,27,42});
        faceMesh1.addIndices({22,23,42});
        faceMesh1.addIndices({23,42,43});
        faceMesh1.addIndices({23,24,43});
        faceMesh1.addIndices({24,43,44});
        
        faceMesh1.addIndices({24,25,44});
        faceMesh1.addIndices({25,26,44});
        faceMesh1.addIndices({26,44,45});
        faceMesh1.addIndices({26,16,45});
        faceMesh1.addIndices({16,45,15});
        faceMesh1.addIndices({45,46,15});
        faceMesh1.addIndices({15,14,46});
        faceMesh1.addIndices({14,47,46});
        faceMesh1.addIndices({29,47,14});
        faceMesh1.addIndices({29,14,13});
        faceMesh1.addIndices({29,13,35});
        faceMesh1.addIndices({35,13,12});
        faceMesh1.addIndices({35,54,12});
        faceMesh1.addIndices({54,12,11});
        
        faceMesh1.addIndices({54,10,11});
        faceMesh1.addIndices({54,55,10});
        faceMesh1.addIndices({55,9,10});
        faceMesh1.addIndices({55,56,9});
        faceMesh1.addIndices({56,8,9});
        faceMesh1.addIndices({57,56,8});
        faceMesh1.addIndices({58,57,8});
        faceMesh1.addIndices({58,7,8});
        faceMesh1.addIndices({59,58,7});
        faceMesh1.addIndices({59,6,7});
        faceMesh1.addIndices({48,59,6});
        faceMesh1.addIndices({48,5,6});
        faceMesh1.addIndices({48,4,5});
        faceMesh1.addIndices({31,48,4});
        faceMesh1.addIndices({31,3,4});
        faceMesh1.addIndices({29,31,3});
        
        faceMesh1.addIndices({29,3,2});
        faceMesh1.addIndices({29,40,2});
        faceMesh1.addIndices({40,41,2});
        faceMesh1.addIndices({2,1,41});
        faceMesh1.addIndices({36,41,1});
        faceMesh1.addIndices({1,0,36});
        faceMesh1.addIndices({40,29,28});
        faceMesh1.addIndices({40,39,28});
        faceMesh1.addIndices({39,27,28});
        faceMesh1.addIndices({27,42,28});
        faceMesh1.addIndices({42,47,28});
        faceMesh1.addIndices({28,47,29});
        faceMesh1.addIndices({29,30,35});
        
        faceMesh1.addIndices({30,34,35});
        faceMesh1.addIndices({30,33,34});
        faceMesh1.addIndices({30,32,33});
        faceMesh1.addIndices({30,31,32});
        faceMesh1.addIndices({29,30,31});
        faceMesh1.addIndices({31,48,49});
        faceMesh1.addIndices({31,49,50});
        faceMesh1.addIndices({31,32,50});
        faceMesh1.addIndices({32,33,50});
        faceMesh1.addIndices({33,50,51});
        faceMesh1.addIndices({33,51,52});
        faceMesh1.addIndices({33,34,52});
        faceMesh1.addIndices({34,35,52});
        faceMesh1.addIndices({35,52,53});
        faceMesh1.addIndices({35,53,54});
        
        faceMesh1.addIndices({48,49,60});
        faceMesh1.addIndices({60,49,61});
        faceMesh1.addIndices({49,50,61});
        faceMesh1.addIndices({61,50,62});
        faceMesh1.addIndices({62,50,51});
        faceMesh1.addIndices({62,61,50});
        faceMesh1.addIndices({62,51,52});
        faceMesh1.addIndices({62,52,63});
        faceMesh1.addIndices({63,52,53});
        faceMesh1.addIndices({63,53,64});
        faceMesh1.addIndices({64,53,54});
        faceMesh1.addIndices({64,54,55});
        faceMesh1.addIndices({64,65,55});
        faceMesh1.addIndices({56,55,65});
        faceMesh1.addIndices({65,66,56});
        faceMesh1.addIndices({66,56,57});
        faceMesh1.addIndices({66,58,57});
        faceMesh1.addIndices({66,67,58});
        faceMesh1.addIndices({58,59,67});
        faceMesh1.addIndices({60,67,59});
        faceMesh1.addIndices({48,60,59});
        
        ////////////////////////////////////
        
        faceMesh2.addIndices({0,17,36});
        faceMesh2.addIndices({17,36,37});
        faceMesh2.addIndices({17,18,37});
        faceMesh2.addIndices({18,37,38});
        faceMesh2.addIndices({18,38,19});
        faceMesh2.addIndices({19,38,20});
        faceMesh2.addIndices({20,38,21});
        faceMesh2.addIndices({21,38,39});
        faceMesh2.addIndices({21,39,27});
        faceMesh2.addIndices({21,22,27});
        faceMesh2.addIndices({22,27,42});
        faceMesh2.addIndices({22,23,42});
        faceMesh2.addIndices({23,42,43});
        faceMesh2.addIndices({23,24,43});
        faceMesh2.addIndices({24,43,44});
        
        faceMesh2.addIndices({24,25,44});
        faceMesh2.addIndices({25,26,44});
        faceMesh2.addIndices({26,44,45});
        faceMesh2.addIndices({26,16,45});
        faceMesh2.addIndices({16,45,15});
        faceMesh2.addIndices({45,46,15});
        faceMesh2.addIndices({15,14,46});
        faceMesh2.addIndices({14,47,46});
        faceMesh2.addIndices({29,47,14});
        faceMesh2.addIndices({29,14,13});
        faceMesh2.addIndices({29,13,35});
        faceMesh2.addIndices({35,13,12});
        faceMesh2.addIndices({35,54,12});
        faceMesh2.addIndices({54,12,11});
        
        faceMesh2.addIndices({54,10,11});
        faceMesh2.addIndices({54,55,10});
        faceMesh2.addIndices({55,9,10});
        faceMesh2.addIndices({55,56,9});
        faceMesh2.addIndices({56,8,9});
        faceMesh2.addIndices({57,56,8});
        faceMesh2.addIndices({58,57,8});
        faceMesh2.addIndices({58,7,8});
        faceMesh2.addIndices({59,58,7});
        faceMesh2.addIndices({59,6,7});
        faceMesh2.addIndices({48,59,6});
        faceMesh2.addIndices({48,5,6});
        faceMesh2.addIndices({48,4,5});
        faceMesh2.addIndices({31,48,4});
        faceMesh2.addIndices({31,3,4});
        faceMesh2.addIndices({29,31,3});
        
        faceMesh2.addIndices({29,3,2});
        faceMesh2.addIndices({29,40,2});
        faceMesh2.addIndices({40,41,2});
        faceMesh2.addIndices({2,1,41});
        faceMesh2.addIndices({36,41,1});
        faceMesh2.addIndices({1,0,36});
        faceMesh2.addIndices({40,29,28});
        faceMesh2.addIndices({40,39,28});
        faceMesh2.addIndices({39,27,28});
        faceMesh2.addIndices({27,42,28});
        faceMesh2.addIndices({42,47,28});
        faceMesh2.addIndices({28,47,29});
        faceMesh2.addIndices({29,30,35});
        
        faceMesh2.addIndices({30,34,35});
        faceMesh2.addIndices({30,33,34});
        faceMesh2.addIndices({30,32,33});
        faceMesh2.addIndices({30,31,32});
        faceMesh2.addIndices({29,30,31});
        faceMesh2.addIndices({31,48,49});
        faceMesh2.addIndices({31,49,50});
        faceMesh2.addIndices({31,32,50});
        faceMesh2.addIndices({32,33,50});
        faceMesh2.addIndices({33,50,51});
        faceMesh2.addIndices({33,51,52});
        faceMesh2.addIndices({33,34,52});
        faceMesh2.addIndices({34,35,52});
        faceMesh2.addIndices({35,52,53});
        faceMesh2.addIndices({35,53,54});
        
        faceMesh2.addIndices({48,49,60});
        faceMesh2.addIndices({60,49,61});
        faceMesh2.addIndices({49,50,61});
        faceMesh2.addIndices({61,50,62});
        faceMesh2.addIndices({62,50,51});
        faceMesh2.addIndices({62,61,50});
        faceMesh2.addIndices({62,51,52});
        faceMesh2.addIndices({62,52,63});
        faceMesh2.addIndices({63,52,53});
        faceMesh2.addIndices({63,53,64});
        faceMesh2.addIndices({64,53,54});
        faceMesh2.addIndices({64,54,55});
        faceMesh2.addIndices({64,65,55});
        faceMesh2.addIndices({56,55,65});
        faceMesh2.addIndices({65,66,56});
        faceMesh2.addIndices({66,56,57});
        faceMesh2.addIndices({66,58,57});
        faceMesh2.addIndices({66,67,58});
        faceMesh2.addIndices({58,59,67});
        faceMesh2.addIndices({60,67,59});
        faceMesh2.addIndices({48,60,59});
        
        ////////////////////////////////////
        
        faceMesh1.drawWireframe();
        faceMesh2.drawWireframe();
        
        //Get texture points
        for (int c = 0; c < landmarks2.size(); c++)
            facePoints1.push_back(ofVec2f(landmarks2[c]));
        for (int c = 0; c < landmarks1.size(); c++)
            facePoints2.push_back(ofVec2f(landmarks1[c]));
        
        faceMesh1.clearTexCoords();
        faceMesh1.addTexCoords(facePoints1);
        
        faceMesh2.clearTexCoords();
        faceMesh2.addTexCoords(facePoints2);
        
        //Draw meshes into Fbos for cloning
        maskFbo1.begin();
        ofClear(0, 255);
        faceMesh1.draw();
        maskFbo1.end();
        
        maskFbo2.begin();
        ofClear(0, 255);
        faceMesh2.draw();
        maskFbo2.end();
    
        srcFbo1.begin();
        ofClear(0, 255);
        vidImg.bind();
        faceMesh1.draw();
        vidImg.unbind();
        srcFbo1.end();
        
        srcFbo2.begin();
        ofClear(0, 255);
        vidImg.bind();
        faceMesh2.draw();
        vidImg.unbind();
        srcFbo2.end();
        
        //Draw the clones
        clone1.setStrength(40);
        clone1.update(srcFbo1.getTexture(), vidImg.getTexture(), maskFbo1.getTexture());
        clone1.draw(0, 0);
        
        vidImg.grabScreen(0, 0, camWidth, camHeight);
        
        clone2.setStrength(40);
        clone2.update(srcFbo2.getTexture(), vidImg.getTexture(), maskFbo2.getTexture());
        clone2.draw(0, 0);
     
    }

    vidGrabber.update();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
