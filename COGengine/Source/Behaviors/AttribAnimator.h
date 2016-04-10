#pragma once

#include "Behavior.h"
#include "TransformEnt.h"
#include "AttrAnimEnt.h"
#include "AnimContextStack.h"
#include "AttribAnim.h"

namespace Cog {


	/**x
	* Behavior for per-attribute animation
	*/
	class AttribAnimator : public Behavior {
	private:
		AnimContextStack contextStack;
	public:
		AttribAnimator() {

		}

		AttribAnimator(spt<SheetAnim> anim) {
			contextStack = AnimContextStack(anim);
		}

		virtual void Load(Setting& setting);

		void OnStart();

		/**
		* Initializes animation entity according to the actual transformation of owner node
		*/
		void InitEntity(AttribAnimContext& animContext);

		void Update(const uint64 delta, const uint64 absolute);
	
	private:

		/**
		* Maps AttrAnimEnt entity into TransformEnt entity that is used in transform calculation
		*/
		TransformEnt CreateEntityFromAttrAnim(spt<AttrAnimEnt> entity, float value);

		/**
		* Gets actual value of the animated property
		*/
		float GetAttrib(AttributeType attr, Trans& transform);

		/**
		* Sets a new value of the animated property
		*/
		void SetAttrib(AttributeType attr, float value, Trans& transform);
	};

}// namespace