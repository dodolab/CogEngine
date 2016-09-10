#pragma once

#include "ofxCogCommon.h"
#include "CommonAnim.h"
#include "AttrAnimEnt.h"

namespace Cog {

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

		void Init(float fromVal, float toVal) {
			this->fromVal = fromVal;
			this->toVal = toVal;
			initialized = true;
		}

		bool IsInitialized() {
			return initialized;
		}

		spt<AttrAnimEnt> GetEntity() {
			return entity;
		}

		float GetFromValue() {
			return fromVal;
		}

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

		void RecalcDuration() {
			for (auto& enti : animEntities) {
				if (enti.GetEntity()->duration > this->duration) {
					this->duration = enti.GetEntity()->duration;
				}
			}
		}

		virtual ~AttribAnim() {
		}


		/**
		* Copies all parameters from other animation
		*/
		void GetParametersFromReference(CommonAnim* reference) {
			auto attribAnimRef = static_cast<AttribAnim*>(reference);

			this->SetDuration(attribAnimRef->GetDuration());
			
			for (auto& ent : attribAnimRef->GetAnimEntities()) {
				AttribAnimContext ctx = AttribAnimContext(ent.GetEntity());
				animEntities.push_back(ctx);
			}

			CommonAnim::GetParametersFromReference(reference);
		}

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

		virtual bool IsMeasurable() {
			return true;
		}

		virtual int GetDuration() {
			return duration;
		}

		void SetDuration(int duration) {
			this->duration = duration;
		}

		bool HasAnimEntities() {
			return this->animEntities.size() != 0;
		}

		vector<AttribAnimContext>& GetAnimEntities() {
			return this->animEntities;
		}
	};



}// namespace