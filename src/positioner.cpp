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
	scServer = ofPtr<ofxSCServer>(new ofxSCServer("192.168.2.50", 57110));


	ofxOscMessage m;
	m.setAddress("/g_new");
	m.addIntArg(1);
	m.addIntArg(0);//position
	m.addIntArg(0);//group ID
	scServer->sendMsg(m);

	calibAnalog = 900;
	phase = PHASE_TENSION;

	//	ev_tension();
#ifndef TARGET_OSX
	pinMode(ADC_SS_PIN, OUTPUT);
#endif
	
	calibAnalog = getAnalog(7);
	requestPos.set(0, -300, 0);
}

void positioner::update()
{
	if (ofGetFrameNum() % 600 == 0)
	{
//		ofPtr<synthObj> obj = ofPtr<synthObj>(new synthObj());
//		obj->setup("s04_test", scServer);
//		obj->setAutoDie(600);
//		obj->create();
//		synthes.push_back(obj);
	}

	if (ofGetFrameNum() % 3 == 0)
	{
		currentAnalog = getAnalog(7);
	}
	
	switch (phase) {
		case PHASE_TENSION:
			if (calibAnalog < 100)//アナログ値が100を下回る場合キャリブレーションを続ける
			{
				calibAnalog = getAnalog(7);
			}
			else
			{
				if (abs(currentAnalog - calibAnalog) > 5)
				{
					ev_attension();
				}
			}
			break;
			
			
		case PHASE_ATTENSION:
			if (synthes.size() == 0) ev_search();
			search_forceCount = 0;
			break;
			
		case PHASE_SEARCH:
			if (ofGetFrameNum() % 120 == 0)
			{
				if ((abs(currentAnalog - calibAnalog) < 2) &&
					(search_forceCount > 5))
				{
					ev_sustain();
				}else{
					search_forceCount++;
					requestPos.set(ofRandomf() * 50,
								   ofRandom(-250, -150),
								   ofRandomf() * 50);
					addNode_Pulse();
					addNode_Pulse();
					addNode_Pulse();
				}
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