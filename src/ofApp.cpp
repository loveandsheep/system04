#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	center = 0;
	sim.setup();
	
	motor.setup(true, motorNum);
	motor.resetDevice();
	
	sleep(1);
	motor.enableAllMotor();
	motor.sendSignal(RPI_L6470_SIG_ACCEL, 0x10);
	motor.sendSignal(RPI_L6470_SIG_DECEL, 0x10);
	motor.sendSignal(RPI_L6470_SIG_MAXSPEED, 0x20);
	motor.sendSignal(RPI_L6470_SIG_MINSPEED, 0x0);
	motor.sendSignal(RPI_L6470_SIG_VOLT_RUN, 0x34);
	motor.sendSignal(RPI_L6470_SIG_VOLT_ACC, 0x34);
	motor.sendSignal(RPI_L6470_SIG_VOLT_DEC, 0x34);
	motor.sendSignal(RPI_L6470_SIG_VOLT_HOLD, 0x34);
	motor.sendSignal(RPI_L6470_SIG_ABSPOS, center);
	motor.sendSignal(RPI_L6470_SIG_GOTO, center);
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
	motor.sendSignal(RPI_L6470_SIG_STEPMODE, 0);
	
	motor_pos.assign(motorNum, center);
	
	receiver.setup(12400);
	sim.update();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if (m.getAddress() == "/reset")
		{
			motor.setup(true, motorNum);
			motor.resetDevice();
			
			sleep(1);
			motor.enableAllMotor();
			motor.sendSignal(RPI_L6470_SIG_ACCEL, 0x10);
			motor.sendSignal(RPI_L6470_SIG_DECEL, 0x10);
			motor.sendSignal(RPI_L6470_SIG_MAXSPEED, 0x20);
			motor.sendSignal(RPI_L6470_SIG_MINSPEED, 0x0);
			motor.sendSignal(RPI_L6470_SIG_VOLT_RUN, 0x34);
			motor.sendSignal(RPI_L6470_SIG_VOLT_ACC, 0x34);
			motor.sendSignal(RPI_L6470_SIG_VOLT_DEC, 0x34);
			motor.sendSignal(RPI_L6470_SIG_VOLT_HOLD, 0x34);
			motor.sendSignal(RPI_L6470_SIG_ABSPOS, center);
			motor.sendSignal(RPI_L6470_SIG_GOTO, center);
			motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
			motor.sendSignal(RPI_L6470_SIG_STEPMODE, 0);
		}
		
		if (m.getAddress() == "/pos")
		{
			sim.work.setGlobalPosition(m.getArgAsFloat(0),
									   m.getArgAsFloat(1),
									   m.getArgAsFloat(2));
			sim.update();
			
			motor_pos[0] = sim.angle_motor[0] / 1.8 * -64.0;
			motor_pos[1] = sim.angle_motor[1] / 1.8 * -64.0;
			motor_pos[2] = sim.angle_motor[2] / 1.8 * -64.0;
			motor.setGo_toMult(motor_pos);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
	camera.begin();
	sim.draw();
	camera.end();
}

void ofApp::exit()
{
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
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
