#pragma once

#include "ofxCogCommon.h"
#include "Sprite.h"

namespace Cog {

	class SpriteEntity {
	public:
		static int idCounter;
		int id;
		Trans transform;
		spt<Sprite> sprite;

		SpriteEntity(spt<Sprite> sprite, Trans transform) : transform(transform), sprite(sprite), id(idCounter++) {

		}
	};

}// namespace