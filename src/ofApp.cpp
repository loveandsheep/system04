#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

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
	motor.sendSignal(RPI_L6470_SIG_ABSPOS, 0);
	motor.sendSignal(RPI_L6470_SIG_GOTO, 0);
	motor.sendSignal(RPI_L6470_SIG_STOP_HARD, 0);
	motor.sendSignal(RPI_L6470_SIG_STEPMODE, 0);
	
	motor_pos.assign(motorNum, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

	sim.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (ofGetFrameNum() % 60 == 0)
	{
		for (int i = 0;i < 3;i++)
		{
			motor_pos[i] = ofRandom(200);
		}
	}
	
	motor.setGo_toMult(motor_pos);
	
	ofBackground(0);
	
	camera.begin();
	sim.draw();
	camera.end();
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
