
#include "MultiAnim.h"
#include "CogEngine.h"
#include "BehaviorEnt.h"

namespace Cog {

	void MultiAnim::Load(Setting& setting) {
		auto animations = setting.GetItemVals("animations");
		this->repeat = setting.GetItemValBool("repeat");
		auto resourceCache = GETCOMPONENT(ResourceCache);

		for (string anim : animations) {
			spt<BehaviorEnt> ent = resourceCache->GetEntityC<BehaviorEnt>(anim);

			if (!ent) throw IllegalArgumentException(string_format("Animation %s not found", anim.c_str()));

			Behavior* behavior = CogGetEntityStorage()->CreateBehaviorPrototype(ent->type);
			if (!ent->setting.Empty()) behavior->Load(ent->setting);
			
			AddAnimation(behavior);
		}
	}

	MultiAnim::~MultiAnim() {
		// delete all behaviors
		for (auto it = animations.begin(); it != animations.end(); ++it) {
			delete (*it);
		}
	}

	void MultiAnim::AddAnimation(Behavior* anim) {
		animations.push_back(anim);
		if (actual == nullptr) actual = anim;
	}

	void MultiAnim::OnStart() {
		if (actual != nullptr) {
			SetOwner(actual, owner);
			actual->Start();
		}
	}


	void MultiAnim::OnMessage(Msg& msg) {
		if (actual != nullptr) actual->OnMessage(msg);
	}

	void MultiAnim::Update(const uint64 delta, const uint64 absolute) {

		if (actual != nullptr) actual->Update(delta, absolute);
		if (actual->IsFinished()) {
			// get next behavior
			auto it = std::find(animations.begin(), animations.end(), actual);

			if ((it + 1) != animations.end()) {
				actual = *(++it);
				SetOwner(actual, owner);
				actual->Start();
			}
			else if (repeat) {
				actual = animations[0];
				Start();
			}
			else {
				Finish();
			}
		}
	}


}// namespace