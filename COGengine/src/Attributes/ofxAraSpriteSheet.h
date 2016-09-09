#pragma once

#include "ofxAreMain.h"
#include "ofxAraSpriteSet.h"


/**
* Entity for sprite sheet settings
*/
class ofxAraSpriteSheet{
private:
	vector<spt<ofxAraSpriteSet>> sprites;
	spt<ofImage> spriteImage;
	string name;

public:
	ofxAraSpriteSheet(string name, spt<ofImage> spriteImage) : name(name),spriteImage(spriteImage){
		sprites = vector<spt<ofxAraSpriteSet>>();
	}

	spt<ofxAraSpriteSet> AddSpriteSet(int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight){
		ofxAraSpriteSet* sprite = new ofxAraSpriteSet(this,offsetX,offsetY,totalFrames,spriteWidth,spriteHeight,spriteSetWidth, spriteSetHeight,
			this->spriteImage->width, this->spriteImage->height);
		spt<ofxAraSpriteSet> sptSprite = spt<ofxAraSpriteSet>(sprite);

		sprites.push_back(sptSprite);

		return sptSprite;
	}

	bool RemoveSprite(spt<ofxAraSpriteSet> sprite){
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

	vector<spt<ofxAraSpriteSet>>& GetSprites(){
		return sprites;
	}
};

