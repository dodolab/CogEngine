
#ifndef ENUSERACT_H
#define ENUSERACT_H


#include "EnInputAct.h"
#include "IwGeomFVec2.h"
#include "Enums.h"
#include <vector>
using namespace std;

class EnUserAct{
private:
	vector<EnInputAct<Act>> _keyActions;
	vector<EnInputAct<CIwFVec2>> _pointActions;

public:

	EnUserAct();

	vector<EnInputAct<Act>>& GetKeyActions() const;
	void SetKeyActions(vector<EnInputAct<Act>>& val);

	vector<EnInputAct<CIwFVec2>>& GetPointActions() const;
	void SetPointActions(vector<EnInputAct<CIwFVec2>>& val);

};


#endif
