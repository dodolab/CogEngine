#include "EnUserAct.h"
#include <vector>

EnUserAct::EnUserAct(){

}

vector<EnInputAct<Act>>& EnUserAct::GetKeyActions() const{
	return vector<EnInputAct<Act>>();
}

void EnUserAct::SetKeyActions(vector<EnInputAct<Act>> val){

}

vector<EnInputAct<CIwFVec2>>& EnUserAct::GetPointActions() const{
	return vector<EnInputAct<CIwFVec2>>();
}

void EnUserAct::SetPointActions(vector<EnInputAct<CIwFVec2>> val){

}

