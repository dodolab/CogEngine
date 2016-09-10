#pragma once

#include "Behavior.h"
#include "Node.h"
#include "TransformEnt.h"
#include "TransformMath.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "EasingFunc.h"
#include "AttrAnimEnt.h"
#include "AnimContextStack.h"
#include "AttribAnim.h"

namespace Cog {


	/**x
	* Behavior for per-attribute animation
	*/
	class AttribAnimator : public Behavior {
		OBJECT_PROTOTYPE_INIT(AttribAnimator)
	private:
		AnimContextStack contextStack;
	public:

		AttribAnimator(spt<SheetAnim> anim) {
			contextStack = AnimContextStack(anim);
		}

		AttribAnimator(Setting& setting) {
			string animation = setting.GetItemVal("animation");
			string renderTypeStr = setting.GetItemVal("render_type");

			auto resCache = GETCOMPONENT(ResourceCache);
			contextStack.SetRootNode(resCache->GetAnimation(animation));
		}

		void OnStart() {
			if (!contextStack.GetRootNode()) {
				CogLogError("Anim", "Animation cant' run, entity is null");
				Finish();
			}
		}

		/**
		* Initializes animation entity according to the actual transformation of owner node
		*/
		void InitEntity(AttribAnimContext& animContext) {

			spt<AttrAnimEnt> animEntity = animContext.GetEntity();
			
			Trans& ownerTrans = owner->GetTransform();
			TransformMath math = TransformMath();

			TransformEnt toTransEnt;
			TransformEnt fromTransEnt;

			float fromVal, toVal;

			if (animEntity->fromValFromActual) {
				// take the actual transformation as the initial value
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

			// calculate both values
			Trans toTrans;
			math.CalcTransform(toTrans, owner, owner->GetParent(), toTransEnt);
			toVal = GetAttrib(animEntity->attributeType, toTrans);

			// set calculated values
			animContext.Init(fromVal, toVal);
		}

		void Update(const uint64 delta, const uint64 absolute) {
			
			// move to the next transformation state
			contextStack.MoveToNext(delta, CogEngine::GetInstance().GetFps());

			if (contextStack.Ended()) {
				Finish();
				SendMessageToListeners(ACT_TRANSFORM_ENDED, 0, nullptr, owner);
			}
			else {

				// get actual animation node
				spt<AttribAnim> actualChild = contextStack.GetContext().GetActualChild();

				// for each animated value (position, scale, rotation,... ) :
				for (auto& attr : actualChild->GetAnimEntities()) {

					spt<AttrAnimEnt> entity = attr.GetEntity();

					// check if this animation should run
					if (entity->begin < contextStack.GetActualProgress() && entity->end > contextStack.GetActualProgress()) {
						
						// get actual progress <0-1>
						float actual = contextStack.GetActualProgress() == 0 ? 0 : contextStack.GetActualProgress() / entity->duration;

						// apply fade function (if declared)
						if (entity->fadeFunction != nullptr) {
							actual = entity->fadeFunction(actual);
						}

						if (!attr.IsInitialized()) {
							this->InitEntity(attr);
						}

						// set the calculated value
						float actualValue = attr.GetFromValue() + (attr.GetToValue() - attr.GetFromValue())*actual;
						Trans& ownerTrans = owner->GetTransform();
						SetAttrib(entity->attributeType, actualValue, ownerTrans);
					}
				}
			}
		}

	private:

		/**
		* Maps AttrAnimEnt entity into TransformEnt entity that is used in transform calculation
		*/
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

		/**
		* Gets actual value of the animated property
		*/
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

		/**
		* Sets a new value of the animated property
		*/
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