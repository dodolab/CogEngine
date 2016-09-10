#include "NodeBuilder.h"
#include "CogEngine.h"

namespace Cog {

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
		string name = xml->getAttribute(":", "name", "");
		auto newBeh = COGEngine.entityStorage->GetBehaviorPrototype(name)->CreatePrototype();
		node->AddBehavior(newBeh);
	}

} // namespace