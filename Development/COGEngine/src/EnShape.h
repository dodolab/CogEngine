#pragma once
#include "MEnums.h"
#include "ofxSmartPointer.h"
#include "EnSprite.h"

/**
* Entity for rendering
*/
class EnShape{
protected:
	RenderType renderType;
public:

	EnShape(){
		renderType = RenderType::NONE;
	}

	EnShape(RenderType renderType) : renderType(renderType){

	}

	RenderType GetRenderType(){
		return renderType;
	}
};

class EnImageShape : public EnShape{
private:
	spt<ofImage> image;
public:

	EnImageShape(spt<ofImage> img) : EnShape(RenderType::IMAGE){
		this->image = img;
	}

	spt<ofImage> GetImage(){
		return image;
	}

	void SetImage(spt<ofImage> img){
		this->image = img;
	}
};

class EnFontShape : public EnShape{
private:
	spt<ofTrueTypeFont> font;

public:

	EnFontShape(spt<ofTrueTypeFont> font) : EnShape(RenderType::TEXT){
		this->font = font;
	}

	spt<ofTrueTypeFont> GetFont(){
		return font;
	}

	void SetFont(spt<ofTrueTypeFont> font){
		this->font = font;
	}
};

class EnSpriteShape : public EnShape{
private:
	spt<EnSprite> sprite;

public:

	EnSpriteShape(spt<EnSprite> sprite) : EnShape(RenderType::SPRITE){
		this->sprite = sprite;
	}

	spt<EnSprite>& GetSprite(){
		return sprite;
	}
};


