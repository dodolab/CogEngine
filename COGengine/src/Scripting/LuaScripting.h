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

		void OnInit();


		/**
		* Stores created lua behavior proxy into collection
		*/
		void StoreBehavior(BehaviorLua* beh) {
			this->behs.push_back(beh);
		}

		/**
		* Creates instance of lua behavior based on its key
		*/
		BehaviorLua* CreateLuaBehavior(StrId key);

		/**
		* Registers component prototype that is called from Extend() function
		*/
		int RegisterBehaviorPrototype(luabridge::lua_State* L);

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}

		void LoadScript(ofFile file);

		void LoadScript(string str);

	protected:
		void LoadAllScripts();
	};

}// namespace