#include "AttribAnim.h"

namespace Cog {

	void AttribAnimContext::Init(float fromVal, float toVal) {
		this->fromVal = fromVal;
		this->toVal = toVal;
		initialized = true;
	}

	AttribAnim::AttribAnim(string name, string ref, float speed, int repeat, bool isRevert) 
		: GeneralAnim(name, ref, speed, repeat, isRevert) {
		RecalcDuration();
	}

	AttribAnim::AttribAnim(string name, string ref, float speed, int repeat, bool isRevert, vector<AttribAnimContext>& animEntities) 
		: GeneralAnim(name, ref, speed, repeat, isRevert), animEntities(animEntities) {
		RecalcDuration();
	}

	void AttribAnim::RecalcDuration() {
		for (auto& enti : animEntities) {
			if (enti.GetEntity()->duration > this->duration) {
				this->duration = enti.GetEntity()->duration;
			}
		}
	}

	void AttribAnim::GetParametersFromReference(spt<GeneralAnim> reference) {
		auto attribAnimRef = static_pointer_cast<AttribAnim>(reference);

		this->SetDuration(attribAnimRef->GetDuration());

		for (auto& ent : attribAnimRef->GetAnimEntities()) {
			AttribAnimContext ctx = AttribAnimContext(ent.GetEntity());
			animEntities.push_back(ctx);
		}

		GeneralAnim::GetParametersFromReference(reference);
	}

	void AttribAnim::LoadAttributesFromXml(spt<ofxXml> xml) {

		GeneralAnim::LoadAttributesFromXml(xml);

		// load attranim nodes
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


} // namespace