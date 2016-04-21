
#include "AttribAnimator.h"
#include "CogEngine.h"
#include "EntityStorage.h"
#include "TransformMath.h"
#include "ResourceCache.h"

namespace Cog {


	void AttribAnimator::Load(Setting& setting) {
		string animation = setting.GetItemVal("animation");
		string renderTypeStr = setting.GetItemVal("render_type");

		if (animation.empty()) CogLogError("AttribAnimator", "Error while loading AttribAnimator, animation attribute not specified");

		auto resCache = GETCOMPONENT(ResourceCache);
		contextStack.SetRootNode(resCache->GetAnimation(animation));
	}

	void AttribAnimator::OnStart() {
		if (!contextStack.GetRootNode()) {
			CogLogError("AttribAnimator", "Animation cant' run, entity is null");
			Finish();
		}
	}


	void AttribAnimator::InitEntity(AttribAnimContext& animContext) {

		spt<AttrAnimEnt> animEntity = animContext.GetEntity();

		Trans& ownerTrans = owner->GetTransform();
		TransformMath math = TransformMath();

		TransformEnt toTransEnt;
		TransformEnt fromTransEnt;

		float fromVal, toVal;

		if (animEntity->takeInitialFromObject) {
			// take the actual transformation as the initial value
			fromVal = GetAttrib(animEntity->attributeType, ownerTrans);
			ownerTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
			toTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->transType == TransformType::DIRECT ?
				animEntity->toVal : (GetAttrib(animEntity->attributeType, ownerTrans) + animEntity->toVal));
		}
		else {
			// create transformation entities that will be used during update
			fromTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->fromVal);
			Trans fromTrans;
			math.CalcTransform(fromTrans, owner, owner->GetParent(), fromTransEnt);
			fromVal = GetAttrib(animEntity->attributeType, fromTrans);
			toTransEnt = CreateEntityFromAttrAnim(animEntity, animEntity->transType == TransformType::DIRECT ? animEntity->toVal : (animEntity->fromVal + animEntity->toVal));
		}

		// calculate both values
		Trans toTrans;
		math.CalcTransform(toTrans, owner, owner->GetParent(), toTransEnt);
		toVal = GetAttrib(animEntity->attributeType, toTrans);

		// set calculated values
		animContext.Init(fromVal, toVal);
	}

	void AttribAnimator::Update(const uint64 delta, const uint64 absolute) {

		// move to the next transformation state according to actual FPS
		contextStack.MoveToNext(delta, CogEngine::GetInstance().GetFps());

		if (contextStack.Ended()) {
			Finish();
			SendMessage(ACT_TRANSFORM_ENDED, owner);
		}
		else {

			// get actual animation node
			spt<AttribAnim> actualChild = static_pointer_cast<AttribAnim>(contextStack.GetContext().GetActualChild());

			// for each animated value (position, scale, rotation,... ) :
			for (auto& attr : actualChild->GetAnimEntities()) {

				spt<AttrAnimEnt> entity = attr.GetEntity();

				// check if this animation should run
				if (entity->begin < contextStack.GetActualProgress() && entity->end > contextStack.GetActualProgress()) {

					// get actual progress in range <0,1>
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


	TransformEnt AttribAnimator::CreateEntityFromAttrAnim(spt<AttrAnimEnt> entity, float value) {
		TransformEnt ent = TransformEnt();

		switch (entity->attributeType) {
		case AttributeType::NONE:
			// nothing to do here
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
		}

		switch (entity->attributeType) {
		case AttributeType::POS_X:
		case AttributeType::POS_Y:
			ent.pType = entity->calcType;
			break;
		case AttributeType::SIZE_X:
		case AttributeType::SIZE_Y:
		case AttributeType::SIZE:
			ent.sType = entity->calcType;
			break;
		}

		return ent;
	}


	float AttribAnimator::GetAttrib(AttributeType attr, Trans& transform) {
		switch (attr) {
		case AttributeType::NONE:
			// nothing to do here
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
		}

		return -1;
	}


	void AttribAnimator::SetAttrib(AttributeType attr, float value, Trans& transform) {
		switch (attr) {
		case AttributeType::NONE:
			// nothing to do here
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
		}
	}
}// namespace