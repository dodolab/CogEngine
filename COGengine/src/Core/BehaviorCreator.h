

namespace Cog {

	class Behavior;

	/**
	* Class that is used to simple register a behavior prototype by REGISTER_BEHAVIOR macro
	* ComponentStorage creates a new BehaviorCreator for each registered type
	*/
	class BehaviorCreator {
	public:
		virtual Behavior* Create() {
			return nullptr;
		}
	};

	/**
	* Generic creator for registered behavior
	* Seer CreateBehaviorPrototype method in ComponentStorage class
	*/
	template<class T>
	class BehaviorCreatorImpl : public BehaviorCreator {
	public:
		virtual Behavior* Create() {
			return new T();
		}
	};
}