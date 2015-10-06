#pragma once

#include "ofxAreMain.h"
#include "ofxAraSprite.h"

/**
* Entity for rendering
*/
class ofxAraShape{
protected:
	RenderType renderType;
public:

	ofxAraShape(){
		renderType = RenderType::NONE;
	}

	ofxAraShape(RenderType renderType) : renderType(renderType){

	}

	RenderType GetRenderType(){
		return renderType;
	}
};

class EnImageShape : public ofxAraShape{
private:
	spt<ofImage> image;
public:

	EnImageShape(spt<ofImage> img) : ofxAraShape(RenderType::IMAGE){
		this->image = img;
	}

	spt<ofImage> GetImage(){
		return image;
	}

	void SetImage(spt<ofImage> img){
		this->image = img;
	}
};

class EnFontShape : public ofxAraShape{
private:
	spt<ofTrueTypeFont> font;

public:

	EnFontShape(spt<ofTrueTypeFont> font) : ofxAraShape(RenderType::TEXT){
		this->font = font;
	}

	spt<ofTrueTypeFont> GetFont(){
		return font;
	}

	void SetFont(spt<ofTrueTypeFont> font){
		this->font = font;
	}
};

class ofxAraSpriteShape : public ofxAraShape{
private:
	spt<ofxAraSprite> sprite;

public:

	ofxAraSpriteShape(spt<ofxAraSprite> sprite) : ofxAraShape(RenderType::SPRITE){
		this->sprite = sprite;
	}

	spt<ofxAraSprite>& GetSprite(){
		return sprite;
	}
};


