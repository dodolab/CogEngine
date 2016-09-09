#ifndef COPTERFACTORY_H
#define COPTERFACTORY_H

#include "GNode.h"
#include "MGameFactory.h"


class CopterFactory : public MGameFactory{

public:
	
	GNode* CreateRoot();
};



#endif