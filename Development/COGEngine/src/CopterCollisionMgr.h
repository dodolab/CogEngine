#ifndef COPTER_COLLISION_MGR_H
#define COPTER_COLLISION_MGR_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"
#include "EnCollision.h"

/**
* Behavior for copter collision manager
*/
class CopterCollisionMgr : public GBehavior{
private:
	CopterFactory* factory;

public:
	CopterCollisionMgr(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::COLLISION_OCURRED)){
		this->factory = factory;
	}

	virtual void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::COLLISION_OCURRED){
			vector<spt<EnCollision>> collisions = msg.GetSourceObject()->GetAttr<vector<spt<EnCollision>>>(Attrs::COLLISIONS);

			for (auto it = collisions.begin(); it != collisions.end(); ++it){
				spt<EnCollision> col = (*it);
				GNode* source = col->first->IsInGroup(States::COLLID_SOURCE) ? col->first : col->second;
				GNode* target = col->first->IsInGroup(States::COLLID_TARGET) ? col->first : col->second;

				int health = target->GetAttr<int>(Attrs::HEALTH);
				health -= 10;
				target->ChangeAttr<int>(Attrs::HEALTH, health);

				SendMessage(Traversation(ScopeType::SCENE, true, true), Actions::HEALTH_CHANGED, nullptr, target);

				// remove projectil
				source->GetParent()->RemoveChild(source);
			}

			// clear collisions
			collisions.clear();
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		


	}


	
};


#endif