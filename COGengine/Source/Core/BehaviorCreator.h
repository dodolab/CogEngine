

namespace Cog {

	class Behavior;

	class BehaviorCreator {
	public:
		virtual Behavior* CreateDefault() {
			return nullptr;
		}
	};

	template<class T>
	class BehaviorCreatorImpl : public BehaviorCreator {
	public:
		virtual Behavior* CreateDefault() {
			return new T();
		}
	};
}