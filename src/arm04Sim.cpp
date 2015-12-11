//
//  arm04Sim.cpp
//  system04
//
//  Created by Ovis aries on 2015/11/27.
//
//

#include "arm04Sim.hpp"

void arm04Sim::setup()
{
	
	radius_motor = 40.0;
	length_elbow = 150.0;
	length_tip = 150.0;
	
	radius_work = 40.0;
	
	for (int i = 0;i < 3;i++)
	{
		motor[i].setParent(axis);
		elbow[i].setParent(motor[i]);
		tip[i]	.setParent(elbow[i]);
		
		plate[i].setParent(work);
		plate[i].setPosition(cos(ofDegToRad(120 * i)) * radius_work, 0,
							 sin(ofDegToRad(120 * i)) * radius_work);
	}
	
	work.setGlobalPosition(0, -180, 0);
	setDefaultArm();
}

void arm04Sim::update()
{
	
//	work.setGlobalPosition(sin(ofGetFrameNum() / 50.0) * 30,
//						   sin(ofGetFrameNum() / 75.3) * 30 - 150,
//						   sin(ofGetFrameNum() / 42.0) * 30);
	
	ofEnableDepthTest();
	
	setDefaultArm();


	for (int i = 0;i < 3;i++)
	{
		float mover = 22.5;
		float root = 45;
		float angle_a, angle_b;
		float dst_a, dst_b;
		float powLT = powf(length_tip, 2.0);
		for (int j = 0;j < 10; j++)
		{
			angle_a = root + mover;
			angle_b = root - mover;
			
			vChild.setParent(motor[i]);
			vChild.setPosition(sin(ofDegToRad(-angle_a)) * -length_elbow,
							   cos(ofDegToRad(-angle_a)) * -length_elbow,0);
			
			dst_a = vChild.getGlobalPosition().squareDistance(plate[i].getGlobalPosition());
			
			vChild.setPosition(sin(ofDegToRad(-angle_b)) * -length_elbow,
							   cos(ofDegToRad(-angle_b)) * -length_elbow,0);
			
			dst_b = vChild.getGlobalPosition().squareDistance(plate[i].getGlobalPosition());
			
			root = abs(dst_a - powLT) < abs(dst_b - powLT) ? angle_a : angle_b;
			mover /= 2.0;
		}
		
		angle_motor[i] = root;
	}
	
	for (int i = 0;i < 3;i++)
	{
		motor[i].roll(angle_motor[i]);
		elbow[i].lookAt(plate[i]);
	}
	
	ofDisableDepthTest();
}

void arm04Sim::draw()
{
	ofNoFill();
	ofDrawBox(0, 0, 0, 50, 50, 50);
	ofFill();
	
	work.draw();
	
	for (int i = 0;i < 3;i++)
	{
		ofPushMatrix();
		ofTranslate(motor[i].getGlobalPosition());
		ofDrawBitmapString(ofToString(i) + ":" + ofToString(angle_motor[i]), 0, 0);
		ofPopMatrix();

		motor[i].draw();
		elbow[i].draw();
		tip[i]	.draw();
		plate[i].draw();
		
		ofLine(motor[i].getGlobalPosition(),
			   elbow[i].getGlobalPosition());
		ofLine(elbow[i].getGlobalPosition(),
			   tip[i].getGlobalPosition());
	}
}

void arm04Sim::setDefaultArm()
{
	
	for (int i = 0;i < 3;i++)
	{
		motor[i].setOrientation(ofQuaternion());
		elbow[i].setOrientation(ofQuaternion());
		tip[i].setOrientation(ofQuaternion());
		
		motor[i].setPosition(cos(ofDegToRad(120 * i)) * radius_motor, 0,
							 sin(ofDegToRad(120 * i)) * radius_motor);
		elbow[i].setPosition(0, -length_elbow, 0);
		tip[i]	.setPosition(0, 0, -length_tip);

		motor[i].pan(-120 * i);
		
		angle_elbow[i] = 0;
		angle_motor[i] = 0;
	}
	
}

void arm04Sim::set_elbow(int num, float angle)
{
	angle_elbow[num % 3] = angle;
}

void arm04Sim::set_motor(int num, float angle)
{
	angle_motor[num % 3] = angle;
}