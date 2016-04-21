#pragma once

#include "Behavior.h"
#include "AnimContextStack.h"
#include "SheetAnim.h"

namespace Cog {


	/**
	* Behavior for sheet animations
	*/
	class SheetAnimator : public Behavior {
	private:
		// context holding actual animation
		AnimContextStack contextStack;
	public:

		SheetAnimator() {

		}

		SheetAnimator(spt<SheetAnim> anim) {
			contextStack = AnimContextStack(anim);
		}

		virtual void Load(Setting& setting);

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace