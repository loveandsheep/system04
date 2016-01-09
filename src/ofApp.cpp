#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	center = 0;
	sim.setup();
	
	motor.setup(true, motorNum);
	resetMotorCommand();
	
	motor_pos.assign(motorNum, center);
	
	receiver.setup(12400);
	sim.update();
	manual = true;
}

void ofApp::resetMotorCommand()
{
	motor.resetDevice();
	
	sleep(1);
	motor.enableAllMotor();
	motor.sendSignal(RPI_L6470_SIG_ACCEL, 0x10);
	motor.sendSignal(RPI_L6470_SIG_DECEL, 0x10);
	motor.sendSignal(RPI_L6470_SIG_MAXSPEED, 0x20);
	motor.sendSignal(RPI_L6470_SIG_MINSPEED, 0x0);
	motor.sendSignal(RPI_L6470_SIG_VOLT_RUN, 0x25);
	motor.sendSignal(RPI_L6470_SIG_VOLT_ACC, 0x25);
	motor.sendSignal(RPI_L6470_SIG_VOLT_DEC, 0x25);
	motor.sendSignal(RPI_L6470_SIG_VOLT_HOLD, 0x25);
	motor.sendSignal(RPI_L6470_SIG_ABSPOS, center);
	motor.sendSignal(RPI_L6470_SIG_GOTO, center);
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
	motor.sendSignal(RPI_L6470_SIG_STEPMODE, 0);
	
	reflesh = false;
	
	posMan.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if (m.getAddress() == "/calib")
		{
			posMan.calibAnalog = posMan.getAnalog(7);
		}
		
		if (m.getAddress() == "/handle")
		{
			motor.disableAllMotor();
			motor.enableMotor(m.getArgAsInt32(0));
			motor.sendSignal(RPI_L6470_SIG_GOTO, m.getArgAsInt32(1));
		}
		
		if (m.getAddress() == "/accel")
		{
			motor.enableAllMotor();
			motor.sendSignal(RPI_L6470_SIG_ACCEL, m.getArgAsInt32(0));
		}

		if (m.getAddress() == "/decel")
		{
			motor.enableAllMotor();
			motor.sendSignal(RPI_L6470_SIG_DECEL, m.getArgAsInt32(0));
		}

		if (m.getAddress() == "/volt")
		{
			motor.enableAllMotor();
			motor.sendSignal(RPI_L6470_SIG_VOLT_RUN, m.getArgAsInt32(0));
			motor.sendSignal(RPI_L6470_SIG_VOLT_ACC, m.getArgAsInt32(0));
			motor.sendSignal(RPI_L6470_SIG_VOLT_DEC, m.getArgAsInt32(0));
			motor.sendSignal(RPI_L6470_SIG_VOLT_HOLD, m.getArgAsInt32(0));
		}
		if (m.getAddress() == "/reset") resetMotorCommand();
		if (m.getAddress() == "/manual") manual = m.getArgAsInt32(0);
		if (m.getAddress() == "/pos")
		{
			sim.work.setGlobalPosition(m.getArgAsFloat(0),
									   m.getArgAsFloat(1),
									   m.getArgAsFloat(2));
			reflesh = true;
		}
	}
	
	sim.update();
	
	if (!manual)
	{
		if (ofGetFrameNum() % 3 == 0)
		{
			sim.work.setGlobalPosition(posMan.requestPos);
			reflesh = true;
		}
	}
	
	if (reflesh)
	{
		reflesh = false;
		motor_pos[0] = sim.angle_motor[0] / 1.8 * -128.0;
		motor_pos[1] = sim.angle_motor[1] / 1.8 * -128.0;
		motor_pos[2] = sim.angle_motor[2] / 1.8 * -128.0;
		motor.enableAllMotor();
		motor.setGo_toMult(motor_pos);
	}
	
	posMan.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (ofGetFrameNum() % 30 == 0)
	{
		cout << "=== Status ===" << endl;
		cout << "pos :" << posMan.requestPos << endl;
		cout << "claibTn :" << posMan.calibAnalog << endl;
		cout << "tension :" << posMan.currentAnalog << endl;
		cout << "phase :" << posMan.phase << endl;
	}
	
	ofBackground(0);
	
	camera.begin();
	sim.draw();
	camera.end();
	
	string info = "Phase :";
	if (posMan.phase == PHASE_TENSION) info += "Tension";
	if (posMan.phase == PHASE_ATTENSION) info += "Attension";
	if (posMan.phase == PHASE_SEARCH) info += "Search";
	if (posMan.phase == PHASE_SUSTAIN) info += "Sustain";
	ofDrawBitmapString(info, 50, 50);
}

void ofApp::exit()
{
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == '1') posMan.ev_tension();
	if (key == '2') posMan.ev_attension();
	if (key == '3') posMan.ev_search();
	if (key == '4') posMan.ev_sustain();
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
