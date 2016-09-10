#pragma once

#include <string>
#include <vector>
#include "ofRectangle.h"
#include "ofxSmartPointer.h"
#include "Sprite.h"
#include "SpriteEntity.h"

using namespace std;

namespace Cog {

	enum class RenderType {
		NONE, IMAGE, RECTANGLE, POLYGON, PLANE, TEXT, SPRITE, MULTISPRITE, LABEL, BOUNDING_BOX
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
			else if (val.compare("label") == 0) {
				return RenderType::LABEL;
			}
			else if (val.compare("bounding_box") == 0) {
				return RenderType::BOUNDING_BOX;
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

		void SetHeight(float height) {
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
		bool noFill = false;

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

		void SetHeight(float height) {
			this->height = height;
		}

		bool IsNoFill() {
			return noFill;
		}

		void SetNoFill(bool noFill) {
			this->noFill = noFill;
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
	protected:
		spt<ofTrueTypeFont> font;
		stringstream stream;
	public:

		Text(spt<ofTrueTypeFont> font, string text) : Shape(RenderType::TEXT) {
			this->font = font;
			stream << text;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}

		float GetWidth() {
			return GetTextWidth();
		}

		float GetHeight() {
			return GetTextHeight();
		
		}

		float GetTextWidth() {
			return font->stringWidth(stream.str());

		}

		float GetTextHeight() {
			return font->stringHeight("Ay");
		}

		string GetText() {
			return stream.str();
		}

		void SetText(string text) {
			stream.str(""); 
			stream << text;
		}

		void AppendText(string text) {
			stream << text;
		}

		void AppendLine(string text) {
			stream << text << endl;
		}
	};

	class Label : public Text {
	protected:
		int labelWidth;
	public:
		
		Label(spt<ofTrueTypeFont> font, string text, int labelWidth) : Text(font, text) {
			this->renderType = RenderType::LABEL;
			this->labelWidth = labelWidth;
		}

		void SetLabelWidth(int width) {
			this->labelWidth = width;
		}

		int GetLabelWidth() {
			return labelWidth;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}
	};

	class SpriteShape : public Shape {
	private:
		Sprite sprite;
		string sheetName;
		spt<SpriteSet> spriteSet;
	public:

		SpriteShape(Sprite& sprite, spt<SpriteSet> spriteSet, string sheetName) : Shape(RenderType::SPRITE), sprite(sprite), 
			spriteSet(spriteSet), sheetName(sheetName) {
		}

		string GetSheetName() {
			return sheetName;
		}

		Sprite& GetSprite() {
			return sprite;
		}

		void SetSprite(Sprite sprite) {
			this->sprite = sprite;
		}

		spt<SpriteSet> GetSpriteSet() {
			return spriteSet;
		}

	};

	class SpritesShape : public Shape {
	private:
		vector<spt<SpriteEntity>> sprites;

		int width = 1;
		int height = 1;
		string sheetName;

	public:
		SpritesShape(string sheetName, vector<spt<SpriteEntity>> sprites) :
			Shape(RenderType::MULTISPRITE), sheetName(sheetName), sprites(sprites) {
			Recalc();
		}

		void Recalc();

		void RefreshZIndex() {
			sort(sprites.begin(), sprites.end(),
				[](const spt<SpriteEntity>  a, const spt<SpriteEntity> b) -> bool
			{
				return a->transform.localPos.z < b->transform.localPos.z;
			});
		}

		vector<spt<SpriteEntity>>& GetSprites() {
			return sprites;
		}

		void RemoveSprite(spt<SpriteEntity> entity) {
			// todo: performance!
			for (auto it = sprites.begin(); it != sprites.end(); ++it) {
				if ((*it)->id == entity->id) {
					sprites.erase(it);
					break;
				}
			}
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

	/**
	* Renderable bounding box
	*/
	class BoundingBox : public Shape {
	private:
		float width = 0;
		float height = 0;
		bool renderable = true;
		ofRectangle boundingBox;
		float margin = 0;

	public:
		BoundingBox(float width, float height, float margin, bool renderable) : width(width), height(height), margin(margin), renderable(renderable),
			Shape(RenderType::BOUNDING_BOX) {
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

		void SetHeight(float height) {
			this->height = height;
		}

		bool IsRenderable() {
			return renderable;
		}

		void SetIsRenderable(bool renderable) {
			this->renderable = renderable;
		}

		float GetMargin() {
			return margin;
		}

		void SetMargin(float margin) {
			this->margin = margin;
		}

		void Recalc(Node* owner);

		ofRectangle GetBoundingBox() {
			return boundingBox;
		}

	};
}// namespace