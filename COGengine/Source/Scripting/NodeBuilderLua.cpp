#include "NodeBuilderLua.h"
#include "NodeLua.h"
#include "BehaviorLua.h"
#include "Scene.h"
#include <LuaBridge.h>

namespace Cog {

	NodeLua* NodeBuilderLua::CreateNode(string name, SceneLua* scene) {
		return new NodeLua(bld.CreateNode(name, scene->scene));
	}

	NodeLua* NodeBuilderLua::LoadNodeFromXml(xml_node& xml, NodeLua* parent, SceneLua* scene) {
		return new NodeLua(bld.LoadNodeFromXml(xml, parent->node, scene->scene));
	}

	NodeLua* NodeBuilderLua::LoadRefNodeFromXml(xml_node& contextXml, string nodeName, NodeLua* parent, SceneLua* scene) {
		return new NodeLua(bld.LoadRefNodeFromXml(contextXml, nodeName, parent->node, scene->scene));
	}

	void NodeBuilderLua::InitLuaMapping(luabridge::lua_State* L) {
		luabridge::getGlobalNamespace(L)
			.beginClass<NodeBuilderLua>("NodeBuilder")
			.addConstructor<void(*)()>()
			.addFunction("CreateNode", &NodeBuilderLua::CreateNode)
			.addFunction("LoadNodeFromXml", &NodeBuilderLua::LoadNodeFromXml)
			.addFunction("LoadRefNodeFromXml", &NodeBuilderLua::LoadRefNodeFromXml)
			.endClass();
	}

} // namespace