
#ifndef ENUSERACT_H
#define ENUSERACT_H


#include "EnInputAct.h"
#include "IwArray.h"
#include "IwGeomFVec2.h"
#include "Enums.h"

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


#endif
