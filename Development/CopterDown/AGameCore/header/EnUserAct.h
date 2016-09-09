
#ifndef ENUSERACT_H
#define ENUSERACT_H

#include <vector>
#include "EnInputAct.h"

class EnUserAct{
private:
	vector<EnInputAct<Act>*> _keyActions;
	vector<EnInputAct<CIwFVec2>*> _pointActions;

public:

	EnUserAct();

	vector<EnInputAct<Act>*> GetKeyActions();
	void SetKeyActiosn(vector<EnInputAct<Act>*> val);

	vector<EnInputAct<CIwFVec2>*> GetPointActions();
	void SetPointActions(vector<EnInputAct<CIwFVec2>*> val);

};

#endif
