#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* Behavior for sequence of animations but can be used for any behaviors as well
	* As soon as the actual behavior finishes, the next in line takes its place
	*/
	class MultiAnim : public Behavior {
	private:
		// actual behavior being updated
		Behavior* actual = nullptr;
		// list of all behaviors
		vector<Behavior*> animations;
		// indicator whether the collection of behaviors
		// should be repeated
		bool repeat = false;

	public:

		MultiAnim() {

		}

		virtual void Load(Setting& setting);

		~MultiAnim();

		void AddAnimation(Behavior* anim);

		/**
		* Returns true, if the collection of behaviors
		* should be repeated when all of them finish
		*/
		bool Repeat() const{
			return repeat;
		}

		/**
		* Sets indicator whether the collection of behaviors
		* should be repeated when all of them finish
		*/
		void SetRepeat(bool repeat) {
			this->repeat = repeat;
		}

		void OnStart();

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};

}// namespace