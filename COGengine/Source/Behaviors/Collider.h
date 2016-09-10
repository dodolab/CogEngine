#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior thats checks collisions Tween two groups of objects
	*/
	class Collider : public Behavior {
	protected:
		// group of collidable objects, groupA is for collidable (bullet) and groupB for hittable (soldier)
		int groupA = 0;
		int groupB = 0;

	public:

		Collider() {

		}

		/**
		* Creates a new collision behavior
		* @param groupA id of group of collidable objects
		* @param groupB id of group of hittable objects
		*/
		Collider(int groupA, int groupB) :
			groupA(groupA), groupB(groupB) {

		}

		/**
		* Creates a new collision behavior
		* @param group id of group of collidable and hittable objects
		*/
		Collider(int group) : groupA(group), groupB(group) {

		}

		void OnStart();

		void Update(const uint64 delta, const uint64 absolute);
	};


}// namespace