#pragma once

#include "StrId.h"
#include "Component.h"
#include "SteeringMath.h"
#include "Scene.h"

using namespace Cog;

/**
 * Component that handles collisions with the ball
 */
class BallCollisionComponent : public Behavior {
public:

	Node* leftPanel;
	Node* rightPanel;
	Node* topPanel;
	Node* paddle;
	Node* bricks;
	ArkanoidModel* model;
	Dynamics* dynamics;

	virtual void OnInit() {
		leftPanel = owner->GetScene()->FindNodeByTag("left_panel");
		rightPanel = owner->GetScene()->FindNodeByTag("right_panel");
		topPanel = owner->GetScene()->FindNodeByTag("top_panel");
		paddle = owner->GetScene()->FindNodeByTag("paddle");
		bricks = owner->GetScene()->FindNodeByTag("bricks");
		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
		dynamics = owner->GetAttr<Dynamics*>(ATTR_MOVEMENT);
	}

	virtual void Update(uint64 delta, uint64 absolute) {
		
		if (dynamics->GetVelocity().lengthSquared() < 0.5f) {
			// zero velocity -> nothing to check
			return;
		}

		auto hitInfo = new HitInfo();
		bool hit = 
			CheckPanelCollision(hitInfo) || 
			CheckPaddleCollision(hitInfo) || 
			CheckBrickCollision(hitInfo);

		if (hit) {
			SendMessage(ARK_EVENT_OBJECT_HIT, hitInfo);
		}

		// check the bottom as well
		if (owner->GetTransform().localPos.y >= 26) { // scene is scaled to be 25 units of height
			SendMessage(ARK_EVENT_BALL_OUTSIDE_AREA);
		}
	}

	/**
	 * Checks collision with all three panels
	 */
	bool CheckPanelCollision(HitInfo* hitInfo) const {
		auto& ballBB = owner->GetMesh()->GetBoundingBox();
		auto& leftPanelBB = leftPanel->GetMesh()->GetBoundingBox();
		auto& rightPanelBB = rightPanel->GetMesh()->GetBoundingBox();
		auto& topPanelBB = topPanel->GetMesh()->GetBoundingBox();
		
		if (ballBB.Intersects(topPanelBB) && dynamics->GetVelocity().y < 0) {
			dynamics->GetVelocity().y *= -1;
			hitInfo->hitType = HIT_TYPE_BORDER;
			return true;
		}

		if (ballBB.Intersects(leftPanelBB) && dynamics->GetVelocity().x < 0) {
			dynamics->GetVelocity().x *= -1;
			hitInfo->hitType = HIT_TYPE_BORDER;
			return true;
		}

		if (ballBB.Intersects(rightPanelBB) && dynamics->GetVelocity().x > 0) {
			hitInfo->hitType = HIT_TYPE_BORDER;
			dynamics->GetVelocity().x *= -1;
			return true;
		}
		return false;
	}

	/**
	 * Checks collision with the paddle
	 */
	bool CheckPaddleCollision(HitInfo* hitInfo) const {
		auto& paddleBB = paddle->GetMesh()->GetBoundingBox();
		auto& ballBB = owner->GetMesh()->GetBoundingBox();

		if (model->ballReleased && ballBB.Intersects(paddleBB) && dynamics->GetVelocity().y > 0) {
			
			// velocity of the paddle isn't taken into account
			float maxDistanceFromCenter = paddleBB.GetSize().x / 2;
			if (maxDistanceFromCenter != 0) {
				float distFromCenter = ballBB.GetCenter().x - paddleBB.GetCenter().x;
				float percDist = distFromCenter / maxDistanceFromCenter;

				float angle = PI / 4 * percDist;  // 45° max
				auto length = dynamics->GetVelocity().length();

				dynamics->GetVelocity().x = length * sin(angle);
				dynamics->GetVelocity().y = -length * cos(angle);
				hitInfo->hitType = HIT_TYPE_PADDLE;
				return true;
			}
		}
		return false;
	}

	/**
	 * Checks collision with all bricks
	 */
	bool CheckBrickCollision(HitInfo* hitInfo) const {

		auto& ballBB = owner->GetMesh()->GetBoundingBox();
		auto multiMesh = bricks->GetMesh<MultiSpriteMesh>();
		auto& sprites = multiMesh->GetSprites();
		auto& velocity = dynamics->GetVelocity();

		for (int i = 0; i < sprites.size(); i++) {
			BoundingBox bb;
			Sprite* spr = sprites[i];
			spr->CalcBoundingBox(bb);
			
			if (bb.Intersects(ballBB)) {
				if (bb.HorizontalIntersection(ballBB) > bb.VerticalIntersection(ballBB)) {
					
					// check whether the ball is really heading in the wrong direction
					if((bb.bottomLeft.y > ballBB.topLeft.y && bb.topLeft.y < ballBB.topLeft.y && velocity.y < 0) 
						|| (bb.topLeft.y < ballBB.bottomLeft.y && bb.bottomLeft.y > ballBB.bottomLeft.y && velocity.y > 0)) {
						velocity.y *= -1;
					}
				}
				else {
					if ((bb.bottomRight.x > ballBB.bottomLeft.x && bb.bottomLeft.x < ballBB.bottomLeft.x && velocity.x < 0)
						|| (bb.bottomLeft.x < ballBB.bottomRight.x && bb.bottomRight.x > ballBB.bottomRight.x && velocity.x > 0)) {
						velocity.x *= -1;
					}
				}

				hitInfo->hitIndex = i;
				hitInfo->hitType = HIT_TYPE_BLOCK;
				return true;
			}
		}
		return false;
	}
};