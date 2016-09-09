#pragma once

#include "ofxSmartPointer.h"
#include <vector>
#include "ofImage.h"
#include "EnSpriteSet.h"

using namespace std;


/**
* Entity for sprite sheet settings
*/
class EnSpriteSheet{
private:
	vector<spt<EnSpriteSet>> sprites;
	spt<ofImage> spriteImage;
	string name;

public:
	EnSpriteSheet(string name, spt<ofImage> spriteImage) : name(name),spriteImage(spriteImage){
		sprites = vector<spt<EnSpriteSet>>();
	}

	spt<EnSpriteSet> AddSpriteSet(int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight){
		EnSpriteSet* sprite = new EnSpriteSet(this,offsetX,offsetY,totalFrames,spriteWidth,spriteHeight,spriteSetWidth, spriteSetHeight,
			this->spriteImage->width, this->spriteImage->height);
		spt<EnSpriteSet> sptSprite = spt<EnSpriteSet>(sprite);

		sprites.push_back(sptSprite);

		return sptSprite;
	}

	bool RemoveSprite(spt<EnSpriteSet> sprite){
		auto found = find(sprites.begin(), sprites.end(), sprite);

		if (found != sprites.end()){
			sprites.erase(found);
			return true;
		}
		else return false;
	}

	string GetName(){
		return name;
	}

	spt<ofImage> GetSpriteImage(){
		return spriteImage;
	}

	vector<spt<EnSpriteSet>>& GetSprites(){
		return sprites;
	}
};

