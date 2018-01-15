#pragma once

#include "Sprite.h"
#include "Transform.h"

namespace Cog {

	/**
	* Sprite instance, consisting of a sprite and its transformation data
	*/
	class SpriteInst {
	public:
		static int idCounter;
		int id;
		Trans transform;
		Sprite sprite;

		SpriteInst(Sprite& sprite, Trans& transform) 
			: transform(transform), sprite(sprite), id(idCounter++) {

		}
	};

}// namespace