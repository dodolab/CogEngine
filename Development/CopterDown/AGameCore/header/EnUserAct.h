
#ifndef ENUSERACT_H
#define ENUSERACT_H


#include "EnInputAct.h"

class EnUserAct{
private:
	CIwArray<EnInputAct<Act>> _keyActions;
	CIwArray<EnInputAct<CIwFVec2>> _pointActions;

public:

	EnUserAct();

	CIwArray<EnInputAct<Act>>& GetKeyActions() const;
	void SetKeyActions(CIwArray<EnInputAct<Act>>& val);

	CIwArray<EnInputAct<CIwFVec2>>& GetPointActions() const;
	void SetPointActions(CIwArray<EnInputAct<CIwFVec2>>& val);

};


EnUserAct::EnUserAct(){

}

CIwArray<EnInputAct<Act>>& EnUserAct::GetKeyActions() const{
	return CIwArray<EnInputAct<Act>>();
}

void EnUserAct::SetKeyActions(CIwArray<EnInputAct<Act>>& val){

}

CIwArray<EnInputAct<CIwFVec2>>& EnUserAct::GetPointActions() const{
	return CIwArray<EnInputAct<CIwFVec2>>();
}

void EnUserAct::SetPointActions(CIwArray<EnInputAct<CIwFVec2>>& val){

}

#endif
