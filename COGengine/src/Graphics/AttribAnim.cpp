#include "AttribAnim.h"
#include "GeneralAnim.h"

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

	void AttribAnim::LoadAttributesFromXml(xml_node& xml) {

		GeneralAnim::LoadAttributesFromXml(xml);

		// load attranim nodes
		for (auto attribAnimNode : xml.children("attranim")) {
			spt<AttrAnimEnt> trans = spt<AttrAnimEnt>(new AttrAnimEnt());
			auto dummySet = Setting();
			trans->LoadFromXml(attribAnimNode, dummySet);
			this->animEntities.push_back(AttribAnimContext(trans));
		}

		RecalcDuration();
	}

	spt<GeneralAnim> AttribAnim::Clone() {
		auto output = spt<AttribAnim>(new AttribAnim(*this));
		return output;
	}


} // namespace