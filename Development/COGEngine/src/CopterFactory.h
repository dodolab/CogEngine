#ifndef COPTERFACTORY_H
#define COPTERFACTORY_H

#include "GNode.h"
#include "MGameFactory.h"
#include "MGameEngine.h"
#include "Enums.h"

class CopterBeh : public GBehavior{

private:


public:
	CopterBeh() : GBehavior(ElemType::MODEL, EnFlags()){

	}

	void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){

		if(!owner->HasState(100)){
			// not initialized
			owner->SetState(100);
			// frame
			owner->AddAttr<int>(2000,0);
		}

		int frame = owner->GetAttr<int>(2000);

		owner->ChangeAttr(2000,frame+1);

		if((frame/4)%2 == 0){
			owner->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/copter1.png"));
		}else{
			owner->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/copter2.png"));
		}

		if(owner->HasState(States::TO_LEFT)){
			owner->GetTransform().Scale.x = -abs(owner->GetTransform().Scale.x);
		}else{
			owner->GetTransform().Scale.x = abs(owner->GetTransform().Scale.x);
		}
	}


};

class CopterFactory : public MGameFactory{
private:
	GNode* root;
	GNode* splash;
public:
	
	GNode* CreateRoot();

	virtual void SwitchToScene(int scene);

	void OnBackgroundhit(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner);

	void Test(GMsg& mojo){

	}
};



#endif