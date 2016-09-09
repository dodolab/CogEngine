#include "GNode.h"

#ifndef GAMECTRL_H
#define GAMECTRL_H

class MGameCtrl{
private:

	GNode* _root;

public:

	GNode* GetRoot(){
		return _root;
	}

	void Init();
};

#endif
