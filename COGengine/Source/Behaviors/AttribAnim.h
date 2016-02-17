#pragma once

#include "Behavior.h"
#include "Node.h"
#include "TransformEnt.h"
#include "TransformMath.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "EasingFunc.h"
#include "AttrAnimEnt.h"

namespace Cog {


	class AttrAnimNode {
	public:
		vector<spt<AttrAnimNode>> children;
		spt<AttrAnimEnt> entity;
	};

	/**
	* Context entity used in animation behavior as
	* it goes through the animation tree
	*/
	class AttrAnimContext {
	public:
		// index of actual loop
		int actualLoop;
		// actual node whose children are just processing
		spt<AttrAnimNode> node;
		// index of actual child being processed
		int actualChildIndex;
		// indicator, if scope of actual node is reverted
		// two reverted scopes give uninverted scope !!
		bool isScopeReverted;
		// indicator, if the node entity is a root node
		bool isRootNode;

		AttrAnimContext() {

		}

		AttrAnimContext(spt<AttrAnimNode> node, bool isScopeReverted, bool isRootNode) {

			this->isRootNode = isRootNode;
			this->actualLoop = 0;
			this->node = node;
			this->isScopeReverted = isScopeReverted;

			if (isScopeReverted) {
				// start at end
				this->actualChildIndex = node->children.size() - 1;
			}
			else {
				// start at the beginning
				this->actualChildIndex = 0;
			}
		}

		/**
		* Gets true, if this child is reverted; depends on actual scope
		* and settings of actual child
		*/
		bool IsChildReverted() {
			return isScopeReverted ^ GetActualChild()->entity->inverted;
		}

		/*
		* Gets actual child being processed
		*/
		spt<AttrAnimNode> GetActualChild() {
			if (isRootNode) return node;
			else return node->children[actualChildIndex];
		}
	};



	/**x
	* Behavior for per-attribute animation
	*/
	class AttribAnim : public Behavior {
	private:
		spt<AttrAnimEnt> animEntity;
		float fromVal = 0;
		float toVal = 0;
		float actual = 0;
		float actualRepeat = 0;
		bool isForwardDirection = true;
	public:

		AttribAnim(spt<AttrAnimEnt> animEntity) : animEntity(animEntity) {
		}

		void Init() {
			actualRepeat = 0;
			actual = 0;
			isForwardDirection = true;
			
			Trans& ownerTrans = owner->GetTransform();
			TransformMath math = TransformMath();

			TransformEnt toTransEnt;
			TransformEnt fromTransEnt;

			if (animEntity->isFixed) {
				fromVal = GetAttrib(animEntity->attributeType, ownerTrans);
				ownerTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
				toTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->measureType == MeasureType::DIRECT ? 
					animEntity->toVal : (GetAttrib(animEntity->attributeType,ownerTrans) + animEntity->toVal));
			}
			else {
				fromTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->fromVal);
				Trans fromTrans;
				math.CalcTransform(fromTrans, owner, owner->GetParent(), fromTransEnt);
				fromVal = GetAttrib(animEntity->attributeType, fromTrans);
				toTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->measureType == MeasureType::DIRECT ? animEntity->toVal : (animEntity->fromVal + animEntity->toVal));
			}

			Trans toTrans;
			math.CalcTransform(toTrans, owner, owner->GetParent(), toTransEnt);
			toVal = GetAttrib(animEntity->attributeType, toTrans);
		}

		void Update(const uint64 delta, const uint64 absolute) {
			
			actual += delta;


			float actualCropped = actual;

			if (actual >= animEntity->duration) {
				actualCropped = animEntity->duration;
			}

			float actualPercent = actualCropped / animEntity->duration;
			if (!isForwardDirection) actualPercent = 1 - actualPercent;
			if (animEntity->fadeFunction != nullptr) {
				actualPercent = animEntity->fadeFunction(actualPercent);
			}
			float actualValue = fromVal + (toVal - fromVal)*actualPercent;
			Trans& ownerTrans = owner->GetTransform();
			SetAttrib(animEntity->attributeType, actualValue, ownerTrans);

			if (actual >= animEntity->duration) {
				if (isForwardDirection && animEntity->biDirectional) {
					isForwardDirection = false; // go back
					actual = 0;
				}
				else {
					actualRepeat++;

					if (animEntity->repeat > actualRepeat || animEntity->isInfinite) {
						Init();
					}
					else {
						Finish();
						SendMessageNoBubbling(ACT_TRANSFORM_ENDED, 0, nullptr, owner);
					}
				}
			}
		}

		TransformEnt CreateEntityFromAttrAnim(spt<AttrAnimEnt> entity, float value) {
			TransformEnt ent = TransformEnt();

			switch (entity->attributeType) {
			case AttributeType::COMMON:
				// todo
				break;
			case AttributeType::POS_X:
				ent.pos.x = value;
				break;
			case AttributeType::POS_Y:
				ent.pos.y = value;
				break;
			case AttributeType::ROTATION:
				ent.rotation = value;
				break;
			case AttributeType::ROT_CENTER_X:
				ent.rotationCentroid.x = value;
				break;
			case AttributeType::ROT_CENTER_Y:
				ent.rotationCentroid.y = value;
				break;
			case AttributeType::SIZE_X:
				ent.size.x = value;
				break;
			case AttributeType::SIZE_Y:
				ent.size.y = value;
				break;
			case AttributeType::SIZE:
				ent.size.x = ent.size.y = value;
				break;
			case AttributeType::SIZE_CENTER_X:
				break;
			case AttributeType::SIZE_CENTER_Y:
				break;
			}

			switch (entity->attributeType) {
			case AttributeType::POS_X:
			case AttributeType::POS_Y:
				ent.pType = entity->transformType;
				break;
			case AttributeType::SIZE_X:
			case AttributeType::SIZE_Y:
			case AttributeType::SIZE:
				ent.sType = entity->transformType;
				break;
			}

			return ent;
		}

		float GetAttrib(AttributeType attr, Trans& transform) {
			switch (attr) {
			case AttributeType::COMMON:
					// todo
					return -1;
			case AttributeType::POS_X:
				return transform.localPos.x;
			case AttributeType::POS_Y:
				return transform.localPos.y;
			case AttributeType::ROTATION:
				return transform.rotation;
			case AttributeType::ROT_CENTER_X:
				return transform.rotationCentroid.x;
			case AttributeType::ROT_CENTER_Y:
				return transform.rotationCentroid.y;
			case AttributeType::SIZE_X:
				return transform.scale.x;
			case AttributeType::SIZE_Y:
				return transform.scale.y;
			case AttributeType::SIZE:
				return transform.scale.x;
			case AttributeType::SIZE_CENTER_X:
				// todo
				return -1;
			case AttributeType::SIZE_CENTER_Y:
				return -1;
			}

			// todo
			return -1;
		}

		void SetAttrib(AttributeType attr, float value, Trans& transform) {
			switch (attr) {
				case AttributeType::COMMON:
					// todo
					break;
				case AttributeType::POS_X:
					transform.localPos.x = value;
					break;
				case AttributeType::POS_Y:
					transform.localPos.y = value;
					break;
				case AttributeType::ROTATION:
					transform.rotation = value;
					break;
				case AttributeType::ROT_CENTER_X:
					transform.rotationCentroid.x = value;
					break;
				case AttributeType::ROT_CENTER_Y:
					transform.rotationCentroid.y = value;
					break;
				case AttributeType::SIZE_X:
					transform.scale.x = value;
					break;
				case AttributeType::SIZE_Y:
					transform.scale.y = value;
					break;
				case AttributeType::SIZE:
					transform.scale.x = transform.scale.y = value;
					break;
				case AttributeType::SIZE_CENTER_X:
					break;
				case AttributeType::SIZE_CENTER_Y:
					break;
			}
		}
	};

}// namespace