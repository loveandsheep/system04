#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	center = 0;
	sim.setup();
	
	motor.setup(true, motorNum);
	resetMotorCommand();
	
	motor_pos.assign(motorNum, center);
	
	receiver.setup(12400);
	sim.work.setGlobalPosition(0, -300, 0);
	sim.update();
	
	ofDirectory dir;
	dir.listDir("parent");
	
	isParent = (dir.getFiles().size() > 0);
	manual = !isParent;
	
	child.setup(CHILD_ADDR, 12400);
	cout << "Child setup succeed" << endl;
	
	logger.setup("192.168.2.124", isParent ? 12800 : 12700);
}

void ofApp::resetMotorCommand()
{
	motor.resetDevice();
	
	sleep(6);
	motor.enableAllMotor();
	motor.sendSignal(RPI_L6470_SIG_ACCEL, 0x03);
	motor.sendSignal(RPI_L6470_SIG_DECEL, 0x03);
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
	
	if (!manual)
	{
		if (ofGetFrameNum() % 20 == 0)
		{
			sim.work.setGlobalPosition(posMan.requestPos);

			if (isParent)
			{
				ofxOscMessage m;
				m.setAddress("/pos");
				m.addFloatArg(posMan.remotePos.x);
				m.addFloatArg(posMan.remotePos.y);
				m.addFloatArg(posMan.remotePos.z);
				child.sendMessage(m);				
			}
			
			reflesh = true;
		}
	}
	
	sim.update();

	if (reflesh)
	{
		reflesh = false;
		motor_pos[0] = sim.angle_motor[0] / 1.8 * -128.0;
		motor_pos[1] = sim.angle_motor[1] / 1.8 * -128.0;
		motor_pos[2] = sim.angle_motor[2] / 1.8 * -128.0;
		motor.enableAllMotor();
		motor.setGo_toMult(motor_pos);
		
		for (int i = 0;i < 3;i++)
			posMan.currentMotor[i] = motor_pos[i];
		
	}
	
	posMan.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (ofGetFrameNum() % 5 == 0)
	{
		sendLog("/log/parent", isParent);
		sendLog("/log/pos", posMan.requestPos);
		sendLog("/log/calib", posMan.calibAnalog);
		sendLog("/log/tension", posMan.analog_smooth);
		sendLog("/log/phase", posMan.phase);
		sendLog("/log/noise", posMan.noiseVal);
		
		ofVec3f mv = ofVec3f(motor_pos[0], motor_pos[1], motor_pos[2]);
		sendLog("/log/motor", mv);
	}
	
	ofBackground(0);
	
	camera.begin();
	sim.draw();
	camera.end();
	
	string info = "Phase :";
	if (posMan.phase == PHASE_TENSION)		info += "Tension";
	if (posMan.phase == PHASE_ATTENSION)	info += "Attension";
	if (posMan.phase == PHASE_SEARCH)		info += "Search";
	if (posMan.phase == PHASE_SUSTAIN)		info += "Sustain";
	ofDrawBitmapString(info, 50, 50);
}

void ofApp::exit()
{
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
	motor.resetDevice();
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
