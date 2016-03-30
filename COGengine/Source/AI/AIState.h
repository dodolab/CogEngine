#pragma once

#include "ofxSmartPointer.h"

class AIState{
protected:
	virtual bool Equals(AIState*)  = 0;
public:
	bool operator ==(AIState* rhs)  { return rhs->Equals(this); }

	// Test for inequality with another hash.
	bool operator !=(AIState* rhs)  { return !rhs->Equals(this); }

	virtual int GetAgentOnTurn() = 0;

	virtual spt<AIState> DeepCopy() = 0;
};