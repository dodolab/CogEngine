#pragma once

#include "Behavior.h"
#include "AnimContextStack.h"

namespace Cog {

	/**x
	* Behavior for sheet animations
	*/
	class SheetAnimator : public Behavior {
		OBJECT_PROTOTYPE_INIT(SheetAnimator)
	private:
		AnimContextStack contextStack;
	public:

		SheetAnimator(spt<SheetAnim> anim) {
			contextStack = AnimContextStack(anim);
		}

		SheetAnimator(Setting& setting);

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace