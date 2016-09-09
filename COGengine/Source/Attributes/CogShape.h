#pragma once

#include "ofxCogMain.h"
#include "CogSprite.h"

namespace Cog {

	/**
	* Entity for rendering
	*/
	class CogShape {
	protected:
		CogRenderType renderType;
	public:

		CogShape() {
			renderType = CogRenderType::NONE;
		}

		CogShape(CogRenderType renderType) : renderType(renderType) {

		}

		CogRenderType GetRenderType() {
			return renderType;
		}
	};

	class CogImage : public CogShape {
	private:
		spt<ofImage> image;
	public:

		CogImage(spt<ofImage> img) : CogShape(CogRenderType::IMAGE) {
			this->image = img;
		}

		spt<ofImage> GetImage() {
			return image;
		}

		void SetImage(spt<ofImage> img) {
			this->image = img;
		}
	};

	class CogFont : public CogShape {
	private:
		spt<ofTrueTypeFont> font;

	public:

		CogFont(spt<ofTrueTypeFont> font) : CogShape(CogRenderType::TEXT) {
			this->font = font;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}
	};

	class CogSpriteShape : public CogShape {
	private:
		spt<CogSprite> sprite;

	public:

		CogSpriteShape(spt<CogSprite> sprite) : CogShape(CogRenderType::SPRITE) {
			this->sprite = sprite;
		}

		spt<CogSprite>& GetSprite() {
			return sprite;
		}
	};


}