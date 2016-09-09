#ifndef RESCTRL_H
#define RESCTRL_H

#include "SmartPointer.h"
#include <string>
#include <map>
#include "ofImage.h"


using namespace std;

class MResourceCtrl{
private:
//	CIwResGroup* resourceGroup;
	map<string, spt<ofImage>> loadedImages;

public:

	void Init();
	void Terminate();
	spt<ofImage> Get2DImage(string name);
};

#endif
