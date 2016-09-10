#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for animation sequence; can store a collection of animations
	*/
	class MultiAnim : public Behavior {
		OBJECT_PROTOTYPE_INIT(MultiAnim)
	private:
		Behavior* actual = nullptr;
		vector<Behavior*> animations;
		bool repeat = false;

	public:

		MultiAnim(Setting setting);

		~MultiAnim();

		void AddAnimation(Behavior* anim);

		bool Repeat() {
			return repeat;
		}

		void SetRepeat(bool repeat) {
			this->repeat = repeat;
		}

		void OnStart();

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};

}// namespace