#pragma once

#include "ofxCogMain.h"
#include "Goal.h"
#include "StrId.h"


class DummyGoal : public Goal {
public:
	bool complete;

	DummyGoal(bool complete) : Goal(StrId()), complete(complete) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (complete) Complete();
		else Fail();
	}
};

