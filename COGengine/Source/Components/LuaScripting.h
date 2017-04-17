#pragma once

#include "Component.h"
#include "LuaMapper.h"

namespace luabridge {
	struct lua_State;
}

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
		// references to behavior prototypes
		map<StrId, int> behaviorPrototypes;

	public:

		LuaScripting() {
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

		/**
		* Creates instance of lua behavior based on its key
		*/
		BehaviorLua* CreateLuaBehavior(StrId key);

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}

		/**
		* Registers behavior prototype that is called from Extend() function
		*/
		int RegisterBehaviorPrototypeCt(luabridge::lua_State* L);

		void LoadScript(ofFile file);

		void LoadScript(string str);

	protected:
		void LoadAllScripts();
	};

}// namespace