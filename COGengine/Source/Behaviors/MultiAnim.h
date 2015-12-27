#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {

	/**x
	* Behavior for animation sequence; can store a collection of animations
	*/
	class MultiAnim : public Behavior {
		OBJECT_PROTOTYPE(MultiAnim)
	private:
		Behavior* actual = nullptr;
		vector<Behavior*> animations;

	public:


		void AddAnimation(Behavior* anim) {
			animations.push_back(anim);
			if (actual == nullptr) actual = anim;
		}

		void Init() {
			RegisterListening(owner->GetScene(), ACT_BEHAVIOR_FINISHED);
			if (actual != nullptr)actual->Init();
			SetOwner(actual, owner);
		}

		void OnMessage(Msg& msg) {

			if (actual != nullptr) actual->OnMessage(msg);

			if (msg.GetAction() == ACT_BEHAVIOR_FINISHED && msg.GetBehaviorId() == actual->GetId()) {
				// get next behavior
				auto it = std::find(animations.begin(), animations.end(), actual);

				if ((it + 1) != animations.end()) {
					actual = *(++it);
					actual->Init();
					SetOwner(actual, owner);
				}
				else Finish();
			}
		}

		void Update(const uint64 delta, const uint64 absolute) {

			if (actual != nullptr) actual->Update(delta, absolute);
		}

	protected:
		~MultiAnim() {
			// delete all behaviors
			for (auto it = animations.begin(); it != animations.end(); ++it) {
				delete (*it);
			}
		}
	};

}// namespace