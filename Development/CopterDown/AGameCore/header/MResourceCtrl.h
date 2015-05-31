#include "SmartPointer.h"
#include "Iw2D.h"
#include <string>
#include "IwResGroup.h"
#include <map>

#ifndef RESCTRL_H
#define RESCTRL_H

using namespace std;

class MResourceCtrl{
private:
	CIwResGroup* resourceGroup;
	map<string, spt<CIw2DImage>> loadedImages;

public:

	void Init();
	void Terminate();
	spt<CIw2DImage> Get2DImage(string name);
	spt<CIwImage> GetImage(string name);
};

#endif
