#pragma once

#include "ofxCogCommon.h"
#include "Sprite.h"

namespace Cog {

	enum class RenderType {
		NONE, IMAGE, RECTANGLE, POLYGON, PLANE, TEXT, SPRITE, MULTISPRITE
	};

	struct RenderTypeConverter {
		static RenderType StrToRenderType(string val) {
			if (val.compare("image") == 0) {
				return RenderType::IMAGE;
			}
			else if (val.compare("rectangle") == 0) {
				return RenderType::RECTANGLE;
			}
			else if (val.compare("polygon") == 0) {
				return RenderType::POLYGON;
			}
			else if (val.compare("text") == 0) {
				return RenderType::TEXT;
			}
			else if (val.compare("plane") == 0) {
				return RenderType::PLANE;
			}
			else if (val.compare("sprite") == 0) {
				return RenderType::SPRITE;
			}
			else if (val.compare("multisprite") == 0) {
				return RenderType::MULTISPRITE;
			}

			return RenderType::NONE;
		}
	};

	/**
	* Entity for rendering
	*/
	class Shape {
	protected:
		RenderType renderType = RenderType::NONE;
		ofColor color;
	public:

		Shape() {
		}

		Shape(RenderType renderType) : renderType(renderType) {

		}

		virtual ~Shape() {

		}

		RenderType GetRenderType() {
			return renderType;
		}

		ofColor GetColor() {
			return color;
		}

		void SetColor(ofColor color) {
			this->color = color;
		}

		virtual float GetWidth() {
			return 1; // must be 1
		}

		virtual float GetHeight() {
			return 1; // must be 1
		}
	};

	/**
	* Not-renderable rectangle
	*/
	class Rectangle : public Shape {
	private:
		float width = 0;
		float height = 0;

	public:
		Rectangle(float width, float height) : width(width), height(height), Shape(RenderType::RECTANGLE) {
		}

		float GetWidth() {
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() {
			return height;
		}

		float SetHeight(float height) {
			this->height = height;
		}
	};

	/**
	* Renderable rectangle
	*/
	class Plane : public Shape {
	private:
		float width = 0;
		float height = 0;

	public:
		Plane(float width, float height) : width(width), height(height), Shape(RenderType::PLANE) {
		}

		float GetWidth() {
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() {
			return height;
		}

		float SetHeight(float height) {
			this->height = height;
		}
	};

	class Image : public Shape {
	private:
		spt<ofImage> image;
	public:

		Image(spt<ofImage> img) : Shape(RenderType::IMAGE) {
			this->image = img; 
		}

		spt<ofImage> GetImage() {
			if (!image->isUsingTexture()) {
				// for preloaded images, they need to be copied to the texture
				image->setUseTexture(true);
				image->update();
			}
			return image;
		}

		void SetImage(spt<ofImage> img) {
			this->image = img;
		}

		float GetWidth() {
			return image->getWidth();
		}

		float GetHeight() {
			return image->getHeight();
		}
	};

	class Text : public Shape {
	private:
		spt<ofTrueTypeFont> font;
		float size;
		string text;

	public:

		Text(spt<ofTrueTypeFont> font, string text, float size) : Shape(RenderType::TEXT) {
			this->font = font;
			this->size = size;
			this->text = text;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}

		float GetSize() {
			return size;
		}

		void SetSize(float size) {
			this->size = size;
		}

		float GetWidth() {
			return GetTextWidth();
		}

		float GetHeight() {
			return GetTextHeight();
		
		}

		float GetTextWidth() {
			return font->stringWidth(text);

		}

		float GetTextHeight() {
			return font->stringHeight(text);
		}

		string GetText() {
			return text;
		}

		void SetText(string text) {
			this->text = text;
		}
	};

	class SpriteShape : public Shape {
	private:
		spt<Sprite> sprite;
		string sheetName;
	public:

		SpriteShape(spt<Sprite> sprite, string sheetName) : Shape(RenderType::SPRITE), sprite(sprite), sheetName(sheetName) {
		}

		string GetSheetName() {
			return sheetName;
		}

		spt<Sprite>& GetSprite() {
			return sprite;
		}

		void SetSprite(spt<Sprite> sprite) {
			this->sprite = sprite;
		}

	};

	class SpritesShape : public Shape {
	private:
		vector<pair<spt<Sprite>, Trans>> sprites;
		int width = 1;
		int height = 1;
		string sheetName;

	public:
		SpritesShape(string sheetName, vector<pair<spt<Sprite>, Trans>> sprites) : 
			Shape(RenderType::MULTISPRITE), sheetName(sheetName), sprites(sprites) {
			Recalc();
		}

		void Recalc();

		vector<pair<spt<Sprite>,Trans>>& GetSprites() {
			return sprites;
		}

		string GetSheetName() {
			return sheetName;
		}

		float GetWidth() {
			return (float)width;
		}

		float GetHeight() {
			return (float)height;

		}
	};

}// namespace