#include "StrId.h"

using namespace std;

namespace Cog {

	/**
	* Structure that provide context for algorithms whose execution can be divided into separate 
	* steps; e.g. for time consuming operations
	*/
	struct CoroutineContext {
		StrId actualState = StrId();
		bool finished = false;
		bool initialized = false;
		function<void(CoroutineContext&)> scope;

		void CallCoroutine() {
			scope(*this);
		}
	};

} // namespace