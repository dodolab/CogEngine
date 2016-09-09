
#ifndef ENUSERACT_H
#define ENUSERACT_H


#include "EnInputAct.h"
#include "IwGeomFVec2.h"
#include "Enums.h"
#include <vector>
using namespace std;

/**
* EnUserAct - storage of user inputs
*/
class EnUserAct{
private:
	// keyboard actions
	vector<EnInputAct<Act>> _keyActions;
	// touchpad actions
	vector<EnInputAct<CIwFVec2>> _pointActions;

public:

	EnUserAct(){

	}

	vector<EnInputAct<Act>>& GetKeyActions(){
		return _keyActions;
	}

	void SetKeyActions(vector<EnInputAct<Act>> val){
		_keyActions = val;
	}

	vector<EnInputAct<CIwFVec2>>& GetPointActions(){
		return _pointActions;
	}

	void SetPointActions(vector<EnInputAct<CIwFVec2>> val){
		_pointActions = val;
	}

};


#endif
