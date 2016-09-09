#pragma once

#include "ofxCogMain.h"

class SpriteSheet;

class SpriteSet{
private:
	int offsetX;
	int offsetY;
	int totalFrames;
	int spriteWidth;
	int spriteHeight;
	int spriteSetWidth;
	int spriteSetHeight;

	SpriteSheet* spriteSheet;

	int spriteSheetWidth;
	int spriteSheetHeight;


public:
	SpriteSet(SpriteSheet* spriteSheet, int offsetX, int offsetY, int totalFrames, 
		int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight, int spriteSheetWidth, int spriteSheetHeight)
		: spriteSheet(spriteSheet), offsetX(offsetX), offsetY(offsetY), totalFrames(totalFrames), spriteWidth(spriteWidth), 
		spriteHeight(spriteHeight), spriteSetWidth(spriteSetWidth), spriteSetHeight(spriteSetHeight),
		spriteSheetWidth(spriteSheetWidth), spriteSheetHeight(spriteSheetHeight){
	}

	int GetOffsetX(){
		return offsetX;
	}

	int GetOffsetY(){
		return offsetY;
	}

	int GetTotalFrames(){
		return totalFrames;
	}

	int GetSpriteWidth(){
		return spriteWidth;
	}

	int GetSpriteHeight(){
		return spriteHeight;
	}

	int GetSpriteSetWidth(){
		return spriteSetWidth;
	}

	int GetSpriteSetHeight(){
		return spriteSetHeight;
	}

	int GetSpriteSheetWidth(){
		return spriteSheetWidth;
	}

	int GetSpriteSheetHeight(){
		return spriteSheetHeight;
	}
};
