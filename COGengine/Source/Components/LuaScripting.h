#pragma once

#include "ofUtils.h"
#include "Component.h"

#include <LuaBridge.h>
#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using namespace luabridge;

namespace Cog {

	class BehaviorLua;
	class NodeLua;
	
	/**
	* Component that within the initialization registers all lua binding classes
	*/
	class LuaScripting : public Component {

	private:
		luabridge::lua_State* L;
		// lua behaviors
		vector<BehaviorLua*> behs;
		// lua nodes
		vector<NodeLua*> nodes;
	public:

		LuaScripting()  {
			this->initPriority = InitPriority::MEDIUM;
		}

		/**
		* Gets pointer to luabridge system
		*/
		luabridge::lua_State* GetLua() {
			return L;
		}

		/**
		* Gets collection of registered lua behaviors
		*/
		vector<BehaviorLua*>& GetBehaviors() {
			return behs;
		}

		/**
		* Gets collection of registered lua nodes
		*/
		vector<NodeLua*>& GetNodes() {
			return nodes;
		}

		void OnInit();
	

		/**
		* Stores created lua behavior proxy into collection
		*/
		void StoreBehavior(BehaviorLua* beh) {
			this->behs.push_back(beh);
		}

		/**
		* Stores created lua node proxy into collection
		*/
		void StoreNode(NodeLua* node) {
			this->nodes.push_back(node);
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}

	protected:
		void LoadAllScripts();
	};

}// namespace