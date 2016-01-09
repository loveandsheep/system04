//
//  positioner.hpp
//  system04
//
//  Created by Ovis aries on 2015/12/10.
//
//

#ifndef positioner_hpp
#define positioner_hpp

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSuperCollider.h"
#include "ofxWiringPi.h"

#define CHILD_ADDR string("192.168.1.107")

#define PHASE_IDLE -1
#define PHASE_TENSION 0
#define PHASE_ATTENSION 1
#define PHASE_SEARCH 2
#define PHASE_SUSTAIN 3

#define ADC_SS_PIN 22

class synthObj;

class positioner{
public:
	
	//TODO: アナログ入力、位置推定アルゴリズム・音響
	
	void setup();
	void update();
	
	//Audio
	void addNode_drone();
	void addNode_tension();
	
	void addNode_Pulse();
	void addNode_whitePerc();
	
	int getAnalog(unsigned char ch);
	
	void removeNode(string name);
	void ev_tension();
	void ev_attension();
	void ev_search();
	void ev_sustain();
	
	int phase;
	float tension;
	
	int calibAnalog;
	int currentAnalog;
	
	ofPtr<ofxSCServer> scServer;
	vector< ofPtr<synthObj> > synthes;
	
	ofVec3f requestPos;
	ofVec3f remotePos;
	int remoteAnalog;
	
	int search_forceCount;
};

class synthObj{
public:
	
	~synthObj()
	{
		synth->free();
	}
	
	ofPtr<ofxSCSynth> synth;
	int age, life;
	bool needRemove;
	string name;
	
	void setup(string type, ofPtr<ofxSCServer> server)
	{
		name = type;
		age = 0;
		life = -1;
		needRemove = false;
		synth = ofPtr<ofxSCSynth>(new ofxSCSynth(type, server.get()));
	}
	
	void create()
	{
		synth->create();
	}
	
	void setAutoDie(int frame)
	{
		life = frame;
	}
	
	void update()
	{
		age++;
		if ((life > 0) && (life < age)) needRemove = true;
	}
};

#endif /* positioner_hpp */
