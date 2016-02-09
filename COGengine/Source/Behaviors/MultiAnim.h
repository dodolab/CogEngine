#pragma once

#include "Behavior.h"
#include "Node.h"
#include "BehaviorEnt.h"

namespace Cog {

	/**x
	* Behavior for animation sequence; can store a collection of animations
	*/
	class MultiAnim : public Behavior {
		OBJECT_PROTOTYPE_INIT(MultiAnim)
	private:
		Behavior* actual = nullptr;
		vector<Behavior*> animations;

	public:

		MultiAnim(Setting setting) {
			auto animations = setting.GetItemVals("animations");
			
			auto resourceCache = GETCOMPONENT(ResourceCache);

			for (string anim : animations) {
				spt<BehaviorEnt> ent = resourceCache->GetEntityC<BehaviorEnt>(anim);

				if (!ent) throw IllegalArgumentException(string_format("Animation %s not found", anim));

				Behavior* prototype = COGEngine.entityStorage->GetBehaviorPrototype(ent->type);
				Behavior* behavior;
				if (!ent->setting.Empty()) behavior = prototype->CreatePrototype(ent->setting);
				else behavior = prototype->CreatePrototype();

				AddAnimation(behavior);
			}
		}

		void AddAnimation(Behavior* anim) {
			animations.push_back(anim);
			if (actual == nullptr) actual = anim;
		}


		void Init() {
			if (actual != nullptr) {
				SetOwner(actual, owner);
				actual->Init();
			}
		}


		void OnMessage(Msg& msg) {
			if (actual != nullptr) actual->OnMessage(msg);
		}

		void Update(const uint64 delta, const uint64 absolute) {

			if (actual != nullptr) actual->Update(delta, absolute);
			if (actual->Ended()) {
				// get next behavior
				auto it = std::find(animations.begin(), animations.end(), actual);

				if ((it + 1) != animations.end()) {
					actual = *(++it);
					SetOwner(actual, owner);
					actual->Init();
				}
				else Finish();
			}
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