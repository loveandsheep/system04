#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "arm04Sim.hpp"
#include "ofxKsmrRPiToL6470.h"
#include "positioner.hpp"

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
	
	void exit();
	void resetMotorCommand();
	
	arm04Sim sim;
	ofEasyCam camera;
	
	static const int motorNum = 3;
	
	int center;
	bool reflesh;
	bool manual;
		
	ofxKsmrRPiToL6470 motor;
	positioner posMan;
	
	vector<int> motor_pos;
	
	ofxOscReceiver receiver;
	
	bool isParent;
	
	ofxOscSender child;
	ofxOscSender logger;
	bool useLogger;
	
	void sendLog(string address, ofVec3f val)
	{
		if (!useLogger) return;
		ofxOscMessage m;
		m.setAddress(address);
		m.addFloatArg(val.x);
		m.addFloatArg(val.y);
		m.addFloatArg(val.z);
		logger.sendMessage(m);
	}
	
	void sendLog(string address, float val)
	{
		if (!useLogger) return;
		ofxOscMessage m;
		m.setAddress(address);
		m.addFloatArg(val);
		logger.sendMessage(m);
	}
};
