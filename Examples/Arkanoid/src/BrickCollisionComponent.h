#pragma once

#include "StrId.h"
#include "Component.h"
#include "SteeringMath.h"
#include "ArkanoidConstants.h"
#include "Scene.h"
#include "ArkanoidModel.h"

using namespace Cog;

/**
 * Resolver for BALL-BRICK collision
 */
class BrickCollisionComponent : public Behavior {
public:
	ArkanoidModel* model;
	Node* bricks;

	virtual void OnInit() {
		SubscribeForMessages(ARK_EVENT_OBJECT_HIT);

		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
		bricks = owner->GetScene()->FindNodeByTag("bricks");
	}

	virtual void OnMessage(Msg& msg) {
		if (msg.GetAction() == ARK_EVENT_OBJECT_HIT) {
			
			auto info = msg.GetDataPtr<HitInfo>();

			if (info->hitType == HIT_TYPE_BLOCK) {
				ResolveBrickHit(info);
			}
		}
	}

	/**
	 * Resolves the collision
	 */
	void ResolveBrickHit(RefCountedObjectPtr<HitInfo> info) {
		auto mesh = bricks->GetMesh<MultiSpriteMesh>();
		auto sprite = mesh->GetSprite(info->hitIndex);
		auto& brick = model->GetBrick(sprite);

		if (brick.type != ARK_BRICK_INDESTRUCTIBLE) {
			// decrement number of bricks
			model->remainingBricks--;

			// remove brick from the model
			auto position = brick.position;
			model->RemoveBrick(position);

			mesh->RemoveSprite(sprite);

			if (model->remainingBricks == 0) {
				// send command message in order to finish the current level
				SendMessage(ARK_COMMAND_FINISH_LEVEL);
			}
		}
	}

	virtual void Update(uint64 delta, uint64 absolute) {

	}
};