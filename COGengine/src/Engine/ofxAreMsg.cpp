#include "ofxAreMsg.h"

int ofxAreMsg::idCounter = 0;

ofxAreMsg::ofxAreMsg(BubblingType bubblingType, int action, int subaction, int behaviorId, ofxAreNode* sourceObj, void* data)
	: bubblingType(bubblingType), action(action), subAction(subaction), behaviorId(behaviorId),
	sourceObj(sourceObj), data(data), id(idCounter++) {
}
