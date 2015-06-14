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
	map<string, spt<ofVboMesh>> loadedMeshes;

public:

	void Init();
	void Terminate();
	spt<ofImage> Get2DImage(string name);
	spt<ofVboMesh> GetMesh(string name);
};

#endif
