#include "SmartPointer.h"
#include "Iw2D.h"
#include <string>
#include "IwResGroup.h"

#ifndef RESCTRL_H
#define RESCTRL_H

using namespace std;

class MResourceCtrl{
private:
	CIwResGroup* resourceGroup;

public:

	void Init();
	void Terminate();
	spt<CIw2DImage> GetImage(string name);
};

#endif
