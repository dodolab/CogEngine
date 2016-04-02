#pragma once

namespace Cog {

	enum class HydroqActionType {
		GO_TO_EMPTY_RIG = 0,
		GO_TO_ENEMY_RIG = 1,
		CAPTURE_EMPTY_RIG = 2,
		CAPTURE_ENEMY_RIG = 3
	};

	class HydroqAction {
	public:
		HydroqActionType type;
		int index;

		HydroqAction() {

		}

		HydroqAction(HydroqActionType type, int index) :type(type), index(index) {

		}

		bool operator !=(const HydroqAction& rhs) { return !(*this == rhs); }

		bool operator ==(const HydroqAction& rhs) {
			return rhs.type == type && rhs.index == index;
		}
	};

} // namespace