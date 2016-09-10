#pragma once

#include "Behavior.h"
#include "AnimContextStack.h"

namespace Cog {

	/**x
	* Behavior for sheet animations
	*/
	class SheetAnimator : public Behavior {
	private:
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