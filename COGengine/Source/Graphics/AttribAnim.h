#pragma once

#include "ofxCogCommon.h"
#include "CommonAnim.h"
#include "AttrAnimEnt.h"

namespace Cog {

	/**
	* Class that holds context of an animation entity
	* Contains from-to values that are calculated according to the 
	* actual state of the transformation node
	*/
	class AttribAnimContext {
	private:
		spt<AttrAnimEnt> entity;
		// real from and to values that are set when animation is initialized
		float fromVal;
		float toVal;
		bool initialized = false;

	public:

		AttribAnimContext(spt<AttrAnimEnt> entity) : entity(entity) {

		}

		/**
		* Initializes the context
		*/
		void Init(float fromVal, float toVal) {
			this->fromVal = fromVal;
			this->toVal = toVal;
			initialized = true;
		}

		/**
		* Returns true, if the context is initialized
		*/
		bool IsInitialized() {
			return initialized;
		}

		/**
		* Gets the animation entity
		*/
		spt<AttrAnimEnt> GetEntity() {
			return entity;
		}

		/**
		* Gets the calculated value the animation begins
		*/
		float GetFromValue() {
			return fromVal;
		}

		/**
		* Gets the calculated value the animation ends
		*/
		float GetToValue() {
			return toVal;
		}
	};

	/**
	* Entity for attribute animations
	*/
	class AttribAnim : public CommonAnim {
	protected:
		vector<AttribAnimContext> animEntities;
		int duration = 0;
	public:

		AttribAnim() {

		}

		AttribAnim(string name, string ref, float speed, int repeat, bool isRevert) : CommonAnim(name,ref,speed,repeat,isRevert) {
			RecalcDuration();
		}

		AttribAnim(string name, string ref, float speed, int repeat, bool isRevert, vector<AttribAnimContext> animEntities) :
			CommonAnim(name, ref, speed, repeat, isRevert), animEntities(animEntities) {
			RecalcDuration();
		}

		virtual ~AttribAnim() {
		}

		/**
		* Recalculates duration as the maximum duration of all children
		*/
		void RecalcDuration() {
			for (auto& enti : animEntities) {
				if (enti.GetEntity()->duration > this->duration) {
					this->duration = enti.GetEntity()->duration;
				}
			}
		}

		/**
		* Copies all parameters from other animation
		*/
		void GetParametersFromReference(spt<CommonAnim> reference) {
			auto attribAnimRef = static_pointer_cast<AttribAnim>(reference);

			this->SetDuration(attribAnimRef->GetDuration());
			
			for (auto& ent : attribAnimRef->GetAnimEntities()) {
				AttribAnimContext ctx = AttribAnimContext(ent.GetEntity());
				animEntities.push_back(ctx);
			}

			CommonAnim::GetParametersFromReference(reference);
		}

		/**
		* Loads attributes from xml
		*/
		virtual void LoadAttributesFromXml(spt<ofxXml> xml) {
			
			CommonAnim::LoadAttributesFromXml(xml);

			int attrAnims = xml->getNumTags("attranim");

			for (int i = 0; i < attrAnims; i++) {
				xml->pushTag("attranim", i);
				spt<AttrAnimEnt> trans = spt<AttrAnimEnt>(new AttrAnimEnt());
				auto dummySet = Setting();
				trans->LoadFromXml(xml, dummySet);
				this->animEntities.push_back(AttribAnimContext(trans));
				xml->popTag();
			}

			RecalcDuration();
		}

		virtual bool IsAnimatable() {
			return HasAnimEntities();
		}

		virtual bool IsContinous() {
			return true;
		}

		virtual int GetDuration() {
			return duration;
		}

		/**
		* Sets the duration
		*/
		void SetDuration(int duration) {
			this->duration = duration;
		}

		/**
		* Returns true, if the animation has entities to animate
		*/
		bool HasAnimEntities() {
			return this->animEntities.size() != 0;
		}

		/**
		* Gets animation entities
		*/
		vector<AttribAnimContext>& GetAnimEntities() {
			return this->animEntities;
		}
	};



}// namespace