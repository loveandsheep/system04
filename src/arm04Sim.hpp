//
//  arm04Sim.hpp
//  system04
//
//  Created by Ovis aries on 2015/11/27.
//
//

#ifndef arm04Sim_hpp
#define arm04Sim_hpp

#include "ofMain.h"

class arm04Sim{
public:
	
	void setup();
	void update();
	void draw();
	
	void setDefaultArm();
	
	float radius_motor;
	float length_elbow;
	float length_tip;
	float radius_work;
	
	ofNode axis;
	ofNode work;
	ofNode plate[3];
	ofNode motor[3];
	ofNode elbow[3];
	ofNode tip[3];
	
	ofMatrix4x4 motor_default[3];
	ofMatrix4x4 elbow_default[3];
	
	float angle_motor[3];
	float angle_elbow[3];
	
	void set_elbow(int num, float angle);
	void set_motor(int num, float angle);
	
	ofNode vChild;

	ofParameterGroup params;
	ofParameter<ofVec3f> targetPos;
	ofParameter<ofVec3f> range;
};

#endif /* arm04Sim_hpp */
