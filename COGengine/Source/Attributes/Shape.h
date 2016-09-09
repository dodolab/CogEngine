#pragma once

#include "ofxCogMain.h"
#include "Sprite.h"

/**
* Entity for rendering
*/
class Shape{
protected:
	RenderType renderType;
public:

	Shape(){
		renderType = RenderType::NONE;
	}

	Shape(RenderType renderType) : renderType(renderType){

	}

	RenderType GetRenderType(){
		return renderType;
	}
};

class EnImageShape : public Shape{
private:
	spt<ofImage> image;
public:

	EnImageShape(spt<ofImage> img) : Shape(RenderType::IMAGE){
		this->image = img;
	}

	spt<ofImage> GetImage(){
		return image;
	}

	void SetImage(spt<ofImage> img){
		this->image = img;
	}
};

class EnFontShape : public Shape{
private:
	spt<ofTrueTypeFont> font;

public:

	EnFontShape(spt<ofTrueTypeFont> font) : Shape(RenderType::TEXT){
		this->font = font;
	}

	spt<ofTrueTypeFont> GetFont(){
		return font;
	}

	void SetFont(spt<ofTrueTypeFont> font){
		this->font = font;
	}
};

class SpriteShape : public Shape{
private:
	spt<Sprite> sprite;

public:

	SpriteShape(spt<Sprite> sprite) : Shape(RenderType::SPRITE){
		this->sprite = sprite;
	}

	spt<Sprite>& GetSprite(){
		return sprite;
	}
};


