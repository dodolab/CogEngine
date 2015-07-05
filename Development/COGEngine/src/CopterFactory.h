#ifndef COPTERFACTORY_H
#define COPTERFACTORY_H

#include "GNode.h"
#include "MGameFactory.h"
#include "MGameEngine.h"
#include "Enums.h"

class CopterFactory : public MGameFactory{
private:
	GNode* root;
	GNode* splash;
	GNode* scene;
public:
	
	CopterFactory(){
		scene = nullptr;
	}

	GNode* CreateRoot();

	virtual void SwitchToScene(int scene);

	void OnBackgroundhit(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner);

	void Test(GMsg const & mojo){

	}

	GNode* CreateBullet(GNode* canon);

	GNode* CreateHelicopter(GNode* scene);

	GNode* CreatePara(GNode* copter);

	void OnButtonHit(GMsg & msg){
		GNode* source = msg.GetSourceObject();
		GNode* scene = source->GetSceneRoot();


		if (!scene->HasAttr(Attrs::ACTIONS)){
			scene->AddAttr(Attrs::ACTIONS, EnFlags());
		}

		EnFlags& actions = scene->GetAttr<EnFlags>(Attrs::ACTIONS);

		if (source->GetTag().compare("but_left") == 0){
			actions.SetState((int)Act::LEFT);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butLeft_press.png"));
		}
		else if (source->GetTag().compare("but_right") == 0){
			actions.SetState((int)Act::RIGHT);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butRight_press.png"));
		}
		else if (source->GetTag().compare("but_fire") == 0){
			actions.SetState((int)Act::FIRE);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butFire_press.png"));
		}

	}

	void OnButtonReleased(GMsg & msg){
		GNode* source = msg.GetSourceObject();
		GNode* scene = source->GetSceneRoot();


		if (!scene->HasAttr(Attrs::ACTIONS)){
			scene->AddAttr(Attrs::ACTIONS, EnFlags());
		}

		EnFlags& actions = scene->GetAttr<EnFlags>(Attrs::ACTIONS);

		if (source->GetTag().compare("but_left") == 0){
			actions.ResetState((int)Act::LEFT);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butLeft.png"));
		}
		else if (source->GetTag().compare("but_right") == 0){
			actions.ResetState((int)Act::RIGHT);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butRight.png"));
		}
		else if (source->GetTag().compare("but_fire") == 0){
			actions.ResetState((int)Act::FIRE);
			source->ChangeAttr(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/butFire.png"));
		}
	}
};



#endif