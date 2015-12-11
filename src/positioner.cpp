//
//  positioner.cpp
//  system04
//
//  Created by Ovis aries on 2015/12/10.
//
//

#include "positioner.hpp"

void positioner::setup()
{
	scServer = ofPtr<ofxSCServer>(new ofxSCServer("localhost", 57110));

#ifndef TARGET_OSX
	ofxOscMessage m;
	m.setAddress("/g_new");
	m.addIntArg(1);
	m.addIntArg(0);//position
	m.addIntArg(1);//group ID
	scServer->sendMsg(m);
#endif
//	ev_tension();
}

void positioner::update()
{
	if (ofGetFrameNum() % 600 == 0)
	{
		ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
		obj->setup("s04_test", scServer);
		obj->setAutoDie(600);
		obj->create();
		synthes.push_back(obj);
	}

	
	switch (phase) {
		case PHASE_TENSION:
			
			break;
			
			
		case PHASE_ATTENSION:
			if (synthes.size() == 0) ev_search();
			break;
			
			
		case PHASE_SEARCH:
			if (ofGetFrameNum() % 120 == 0)
			{
				addNode_Pulse();
				addNode_Pulse();
				addNode_Pulse();
			}
			break;
			
			
		case PHASE_SUSTAIN:
			if (synthes.size() == 0) ev_tension();
			break;
			
	
		default:
			break;
	}
	
	for (int i = 0;i < synthes.size();i++) synthes[i]->update();
	
	vector< ofPtr<synthObj> >::iterator it = synthes.begin();
	while (it != synthes.end())
	{
		if ((*it)->needRemove)
		{
			it = synthes.erase(it);
		}
		else
		{
			++it;
		}
	}
	
}

int positioner::getAnalog(unsigned char ch)
{
#ifndef TARGET_OSX
	digitalWrite(ADC_SS_PIN, 0);
	
	unsigned char sig = 0x01;
	unsigned char data[2];
	
	data[0] = (ch << 4) | 0x80;
	data[1] = 0x0;
	
	wiringPiSPIDataRW(0, &sig, 1);//スタートビット
	wiringPiSPIDataRW(0, &data[0], 1);
	wiringPiSPIDataRW(0, &data[1], 1);
	
	int ret = ((data[0] & 0x03) << 8) | data[1];

	digitalWrite(ADC_SS_PIN, 1);

	return ret;
	
#else
	
	return 0.0;
	
#endif
}

void positioner::ev_tension()
{
	for (int i = 0;i < synthes.size();i++) synthes[i]->needRemove = true;
	addNode_drone();
	phase = PHASE_TENSION;
	
}

void positioner::ev_attension()
{
	for (int i = 0;i < synthes.size();i++) synthes[i]->needRemove = true;
	addNode_whitePerc();
	phase = PHASE_ATTENSION;
	
}

void positioner::ev_search()
{
	phase = PHASE_SEARCH;
}

void positioner::ev_sustain()
{
	for (int i = 0;i < synthes.size();i++) synthes[i]->needRemove = true;
	addNode_whitePerc();
	phase = PHASE_SUSTAIN;
}

void positioner::removeNode(string name)
{
	vector< ofPtr<synthObj> >::iterator it = synthes.begin();
	while (it != synthes.end())
	{
		if ((*it)->name == name)
		{
			it = synthes.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void positioner::addNode_drone()
{
	ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
	obj->setup("s04_drone", scServer);
	obj->synth->set("freq", ofRandom(60, 100));
	obj->create();
	synthes.push_back(obj);
}

void positioner::addNode_Pulse()
{
	ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
	obj->setup("s04_Pulse", scServer);
	obj->synth->set("freq", ofRandom(8000, 18000));
	obj->setAutoDie(120);
	obj->create();
	synthes.push_back(obj);
}

void positioner::addNode_tension()
{
	ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
	obj->setup("s04_Pulse", scServer);
	obj->synth->set("freq", ofRandom(400, 24000));
	obj->create();
	synthes.push_back(obj);
}

void positioner::addNode_whitePerc()
{
	ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
	obj->setup("s04_whitePerc", scServer);
	obj->setAutoDie(300);
	obj->create();
	synthes.push_back(obj);
}