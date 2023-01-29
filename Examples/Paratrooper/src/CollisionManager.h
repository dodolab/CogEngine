#pragma once

#include "Component.h"
#include "Behavior.h"
#include "TransformBuilder.h"
#include "GameValues.h"
#include "Scene.h"

using namespace Cog;

/**
 * Trigger that holds both unit and its colliding projectile
 */
struct CollisionTrigger : MsgPayload {

	CollisionTrigger(Node* unit, Node* projectile) : unit(unit), projectile(projectile) {}

	Node* unit;
	Node* projectile;
};

/**
 * Manager that handles collisions
 */
class CollisionManager : public Behavior {
private:
	vector<Node*> units; // list of all units
	vector<Node*> projectiles; // list of all projectiles
public:

	virtual void OnInit() {
		// we need to refresh both collections whenever a game object is either
		// added or removed. A bit ineffective, however... what the hell
		SubscribeForMessages(ACT_OBJECT_CREATED, ACT_OBJECT_REMOVED);
	}

	virtual void OnMessage(Msg& msg) {
		if (msg.GetAction() == ACT_OBJECT_CREATED || msg.GetAction() == ACT_OBJECT_REMOVED) {
			projectiles.clear();
			units.clear();
			// refresh both collections
			owner->GetScene()->FindNodesByFlag(FLAG_PROJECTILE, projectiles);
			owner->GetScene()->FindNodesByFlag(FLAG_COLLIDABLE, units);
		}
	}


	virtual void Update(uint64_t delta, uint64_t absolute) {

		vector<RefCountedObjectPtr<CollisionTrigger>> collides;

		// O(m^n), we don't suppose there will be more than 50 units in total
		for (auto projectile : projectiles) {
			if (!projectile->HasState(FLAG_DEAD)) {
				for (auto unit : units) {
					if (!unit->HasState(FLAG_DEAD)) {
						if (Collides(projectile, unit)) {
							COGLOGDEBUG("CollisionManager", "Collision with %s", unit->GetTag().c_str());
							collides.push_back((new CollisionTrigger(unit, projectile )));
						}
					}
				}
			}
		}

		// for each colliding pair, send a message
		for (auto collid : collides) {
			SendMessage(COLLISION, collid);
		}
	}

private:
	bool Collides(Node* projectile, Node* unit) {
		auto& unitBB = unit->GetMesh()->GetBoundingBox();
		
		auto& projTrans = projectile->GetTransform();
		
		// check bounding boxes
		return projTrans.absPos.x >= unitBB.topLeft.x && projTrans.absPos.x <= unitBB.topRight.x &&
			projTrans.absPos.y >= unitBB.topLeft.y && projTrans.absPos.y <= unitBB.bottomLeft.y;
	}
};