#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Clone.h"
#include "ofxDlib.h"
#include "FaceTracker.h"

namespace ofxDLib {
    float trackingDistance(const Face& a, const Face& b) {
        ofVec3f aCenter = a.rect.getCenter();
        ofVec3f bCenter = b.rect.getCenter();
        return aCenter.distance(bCenter);
    };
    
    
}

class FaceAugmented : public ofxDLib::Follower<ofxDLib::Face> {
public:
    
    ofImage image;
    ofColor color;
    ofRectangle roi;
    ofVec2f cur, smooth;
    ofPolyline all;
    ofxDLib::Face face;
    FaceAugmented(){
    };
    void setup(const ofxDLib::Face & track);
    void update(const ofxDLib::Face & track);
    void setImage(const ofPixels & pixels);
    void kill();
    void draw();
    
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber vidGrabber;
        ofPixels vidPixel;
        ofImage vidImg;
    
        int camWidth;
        int camHeight;
    
        bool isTracking;
    
        ofImage src;
    
        ofMesh faceMesh1;
        ofMesh faceMesh2;
    
        //vector<ofVec2f> facePoints1, facePoints2;
    
        bool cloneReady;
        Clone clone1, clone2;
        ofFbo maskFbo1, maskFbo2, srcFbo1, srcFbo2;
    
        ofxDLib::FaceTracker ft;
        ofxDLib::TrackerFollower<ofxDLib::Face, FaceAugmented> tracker;
    
        //vector<ofVec3f> landmarks1;
        //vector<ofVec3f> landmarks2;
    
};