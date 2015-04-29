#include "EnUserAct.h"

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

