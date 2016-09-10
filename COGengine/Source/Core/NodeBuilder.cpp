#include "NodeBuilder.h"
#include "CogEngine.h"

namespace Cog {

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
		string name = xml->getAttributex("name", "");
		auto newBeh = COGEngine.entityStorage->GetBehaviorPrototype(name)->CreatePrototype();
		newBeh->LoadFromXml(xml);
		node->AddBehavior(newBeh);
	}

} // namespace