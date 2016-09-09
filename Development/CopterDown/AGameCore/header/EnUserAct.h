
#ifndef ENUSERACT_H
#define ENUSERACT_H


#include "EnInputAct.h"

class EnUserAct{
private:
	CIwArray<EnInputAct<Act>*> _keyActions;
	CIwArray<EnInputAct<CIwFVec2>*> _pointActions;

public:

	EnUserAct();

	CIwArray<EnInputAct<Act>*> GetKeyActions();
	void SetKeyActiosn(CIwArray<EnInputAct<Act>*> val);

	CIwArray<EnInputAct<CIwFVec2>*> GetPointActions();
	void SetPointActions(CIwArray<EnInputAct<CIwFVec2>*> val);

};


EnUserAct::EnUserAct(){

}

CIwArray<EnInputAct<Act>*> EnUserAct::GetKeyActions(){
	return CIwArray<EnInputAct<Act>*>();
}

void EnUserAct::SetKeyActiosn(CIwArray<EnInputAct<Act>*> val){

}

CIwArray<EnInputAct<CIwFVec2>*> EnUserAct::GetPointActions(){
	return CIwArray<EnInputAct<CIwFVec2>*>();
}

void EnUserAct::SetPointActions(CIwArray<EnInputAct<CIwFVec2>*> val){

}

#endif
