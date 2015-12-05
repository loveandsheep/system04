#pragma once

#include "ofMain.h"
#include "arm04Sim.hpp"
#include "ofxKsmrRPiToL6470.h"
#include "ofxOsc.h"

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
	
	arm04Sim sim;
	ofEasyCam camera;
	
	static const int motorNum = 3;
	static const int center = 12800;
	ofxKsmrRPiToL6470 motor;
	
	vector<int> motor_pos;
	
	ofxOscReceiver receiver;
};
