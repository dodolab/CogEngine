#include "GeneralAnim.h"
#include "Error.h"
#include "Utils.h"

namespace Cog {

	void GeneralAnim::GetParametersFromReference(spt<GeneralAnim> reference) {
		this->SetSpeed(reference->GetSpeed());
		this->SetRepeat(reference->GetRepeat());
		this->SetIsRevert(reference->GetIsRevert());

		// insert children
		vector<spt<GeneralAnim>>& refChildren = reference->children;

		for (auto it = refChildren.begin(); it != refChildren.end(); ++it) {
			spt<GeneralAnim> child = (*it);
			AddChild(child);
		}
	}

	void GeneralAnim::GetAllNodes(vector<GeneralAnim*> &output) {
		output.push_back(this);

		for (auto it = children.begin(); it != children.end(); ++it) {
			spt<GeneralAnim> an = (*it);
			an->GetAllNodes(output);
		}
	}

	spt<GeneralAnim> GeneralAnim::FindChild(string name) { 
		if (this->GetName().compare(name) == 0) return this->shared_from_this();

		for (auto it = children.begin(); it != children.end(); ++it) {
			if ((*it)->GetName().compare(name) == 0) return (*it);
			else {
				auto childFound = (*it)->FindChild(name);
				if (childFound) return childFound;
			}
		}

		return spt<GeneralAnim>();
	}

	bool GeneralAnim::AddChild(spt<GeneralAnim> child) {
		auto found = find(children.begin(), children.end(), child);
		if (found != children.end()) {
			return false;
		}
		else {
			children.push_back(child);
			return true;
		}
	}

	bool GeneralAnim::RemoveChild(spt<GeneralAnim> child) {
		auto found = find(children.begin(), children.end(), child);
		if (found != children.end()) {
			children.erase(found);
			return true;
		}
		else return false;
	}

	void GeneralAnim::LoadBaseAttributesFromXml(xml_node& xml) {
		this->SetRef(xml.attribute("ref").as_string(this->GetRef().c_str()));
		this->SetName(xml.attribute("name").as_string(this->GetName().c_str()));
	}

	void GeneralAnim::LoadAttributesFromXml(xml_node& xml) {
		this->SetSpeed(xml.attribute("speed").as_float(this->GetSpeed()));
		if (this->GetSpeed() < 0) throw IllegalArgumentException(string_format("Error in animation %s; speed bust be greater than 0", this->GetName().c_str()));
		this->SetRepeat(xml.attribute("repeat").as_int(this->GetRepeat()));
		if (this->GetRepeat() < 0) throw IllegalArgumentException(string_format("Error in animation %s; number of repetitions must be greater or equal to 0", this->GetName().c_str()));
		this->SetIsRevert(xml.attribute("revert").as_bool(this->GetIsRevert()));
	}

} // namespace