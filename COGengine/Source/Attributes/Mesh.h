#pragma once

#include <string>
#include <vector>
#include "ofRectangle.h"
#include "Definitions.h"
#include "Sprite.h"
#include "SpriteInst.h"

using namespace std;

namespace Cog {

	/**
	* Type of a mesh
	*/
	enum class MeshType {
		NONE,				/** Undefined */
		IMAGE,				/** 2D image */
		RECTANGLE,			/** Unrenderable rectangle */
		PLANE,				/** Renderable rectangle */
		TEXT,				/** 2D text */
		LABEL,				/** Text not affected by transformations */
		SPRITE,				/** 2D sprite */
		MULTISPRITE,		/** Collection of sprites */
		BOUNDING_BOX		/** Box sizeable due to children nodes */
	};

	class Node;

	/**
	* Common class for meshes
	*/
	class Mesh {
	protected:
		MeshType meshType = MeshType::NONE;
		ofColor color;
	public:

		Mesh() {
		}

		Mesh(MeshType meshType) : meshType(meshType) {

		}

		virtual ~Mesh() {

		}

		/**
		* Gets type of the mesh
		*/
		MeshType GetMeshType() const {
			return meshType;
		}

		ofColor GetColor() const {
			return color;
		}

		void SetColor(ofColor color) {
			this->color = color;
		}

		/**
		* Gets width of the mesh
		* If undefined, it returns 1
		*/
		virtual float GetWidth() const {
			return 1; 
		}

		/**
		* Gets height of the mesh
		* If undefined, it returns 1
		*/
		virtual float GetHeight() const {
			return 1; 
		}
	};


	/**
	* Nonrenderable rectangle, used primarily for 
	* transformation calculations
	*/
	class Rectangle : public Mesh {
	private:
		float width = 0;
		float height = 0;

	public:
		Rectangle(float width, float height) 
			: width(width), height(height), Mesh(MeshType::RECTANGLE) {
		}

		float GetWidth() const{
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() const {
			return height;
		}

		void SetHeight(float height)  {
			this->height = height;
		}
	};

	/**
	* Renderable rectangle
	*/
	class Plane : public Mesh {
	private:
		float width = 0;
		float height = 0;
		// indicator, if only borders should be drawn
		bool noFill = false;

	public:
		Plane(float width, float height) 
			: width(width), height(height), Mesh(MeshType::PLANE) {
		}

		float GetWidth() const {
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() const {
			return height;
		}

		void SetHeight(float height) {
			this->height = height;
		}

		/**
		* Gets indicator whether only borders should be drawn
		*/
		bool IsNoFill() const {
			return noFill;
		}

		/**
		* Sets the indicator whether only borders should be drawn
		*/
		void SetNoFill(bool noFill) {
			this->noFill = noFill;
		}
	};

	/**
	* 2D image, wrapper for ofImage
	*/
	class Image : public Mesh {
	private:
		spt<ofImage> image;
	public:

		Image(spt<ofImage> img) : Mesh(MeshType::IMAGE) {
			this->image = img; 
		}

		spt<ofImage> GetImage(){
			if (!image->isUsingTexture()) {
				// preloaded images must be updated before the first use
				image->setUseTexture(true);
				image->update();
			}
			return image;
		}

		void SetImage(spt<ofImage> img) {
			this->image = img;
		}

		float GetWidth() const {
			return image->getWidth();
		}

		float GetHeight() const {
			return image->getHeight();
		}
	};

	/**
	* 2D text
	*/
	class Text : public Mesh {
	protected:
		spt<ofTrueTypeFont> font;
		stringstream stream;
	public:

		Text(spt<ofTrueTypeFont> font, string text) : Mesh(MeshType::TEXT) {
			this->font = font;
			stream << text;
		}

		spt<ofTrueTypeFont> GetFont() const {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}

		float GetWidth() const {
			return GetTextWidth();
		}

		float GetHeight() const {
			return GetTextHeight();
		
		}

		/**
		* Gets raster width of the current string
		*/
		float GetTextWidth() const {
			return font->stringWidth(stream.str());

		}

		/**
		* Gets raster height of the current string
		*/
		float GetTextHeight() const {
			// Height of Ay pair should cover the height
			// of all possible bounding boxes
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

	/**
	* Text not affected by transformations
	*/
	class Label : public Text {
	protected:
		int labelWidth;
	public:
		
		/**
		* Creates a new label
		* @param font font of the label
		* @param text text of the label
		* @param labelWidth absolute width of the label in pixels
		*/
		Label(spt<ofTrueTypeFont> font, string text, int labelWidth) : Text(font, text) {
			this->meshType = MeshType::LABEL;
			this->labelWidth = labelWidth;
		}

		/**
		* Sets absolute label width in pixels
		*/
		void SetLabelWidth(int width) {
			this->labelWidth = width;
		}

		/**
		* Gets absolute label width in pixels
		*/
		int GetLabelWidth() const {
			return labelWidth;
		}

		spt<ofTrueTypeFont> GetFont() const {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}
	};

	/**
	* 2D sprite that is a part of a spritesheet with given frame index
	*/
	class SpriteShape : public Mesh {
	private:
		// sprite entity
		Sprite sprite;
		// sprite set this sprite makes a part
		spt<SpriteSet> spriteSet;
		// name of layer or sprite sheet this shape belongs to
		string layerName;
	public:

		SpriteShape(Sprite& sprite, spt<SpriteSet> spriteSet, string layerName)
			: Mesh(MeshType::SPRITE), sprite(sprite), spriteSet(spriteSet), layerName(layerName) {
		}

		/**
		* Gets name of layer or sprite sheet this sprite belongs to
		* Note that the name could differ from the name of original sprite sheet
		*/
		string GetLayerName() const {
			return layerName;
		}

		Sprite& GetSprite() {
			return sprite;
		}

		void SetSprite(Sprite sprite) {
			this->sprite = sprite;
		}

		spt<SpriteSet> GetSpriteSet(){
			return spriteSet;
		}

	};

	/**
	*  Collection of sprites
	*/
	class MultiSpriteShape : public Mesh {
	private:
		vector<spt<SpriteInst>> sprites;
		
		// width that is recalculated with each new sprite
		int width = 1;
		// height that is recalculated with each new sprite
		int height = 1;
		// name of the layer or sprite sheet this sprites is made of
		string layerName;
		
	public:
		MultiSpriteShape(string layerName)
			: Mesh(MeshType::MULTISPRITE), layerName(layerName) {
		}

		MultiSpriteShape(string layerName, vector<spt<SpriteInst>>& sprites)
			: Mesh(MeshType::MULTISPRITE), layerName(layerName), sprites(sprites) {
			Recalc();
		}

		vector<spt<SpriteInst>>& GetSprites() {
			return sprites;
		}

		/**
		* Recalculates size of this shape, according to collection of sprites
		*/
		void Recalc();

		/**
		* Sorts all sprites by their z-index, starting at the furthest
		*/
		void RefreshZIndex();

		/**
		* Adds a new sprite 
		* Note: don't forget to call RefreshZIndex when finish 
		*/
		void AddSprite(spt<SpriteInst> entity) {
			sprites.push_back(entity);
		}

		/**
		* Removes sprite
		*/
		void RemoveSprite(spt<SpriteInst> entity) {
			auto found = find(sprites.begin(), sprites.end(), entity);
			if (found != sprites.end()) {
				sprites.erase(found);
			}
		}

		string GetLayerName() {
			return layerName;
		}

		/**
		* Gets width of the whole sprite set
		*/
		float GetWidth() const {
			return (float)width;
		}

		/**
		* Gets height of the whole sprite set
		*/
		float GetHeight() const {
			return (float)height;
		}
	};

	/**
	* Renderable bounding box that sets its size according to children
	* of the selected node
	*/
	class BoundingBox : public Mesh {
	private:
		float width = 0;
		float height = 0;
		// indicator, if the box should be rendered
		bool renderable = true;
		ofRectangle boundingBox;
		// margin in percentage size
		float margin = 0;

	public:
		/**
		* Creates a new bounding box
		* @param width width of the bounding box
		* @param height height of the bounding box
		* @param margin margin in percentage size of the inner box
		* @param renderable indicator whether this box should be rendered
		*/
		BoundingBox(float width, float height, float margin, bool renderable) 
			: width(width), height(height), margin(margin), renderable(renderable),
			Mesh(MeshType::BOUNDING_BOX) {
		}

		float GetWidth() const {
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() const {
			return height;
		}

		void SetHeight(float height) {
			this->height = height;
		}

		/**
		* Gets indicator whether this box should be rendered
		*/
		bool IsRenderable() const {
			return renderable;
		}

		/**
		* Sets the indicator whether this box should be rendered
		*/
		void SetIsRenderable(bool renderable) {
			this->renderable = renderable;
		}

		/**
		* Gets magin in percentage size of the inner box
		*/
		float GetMargin() const {
			return margin;
		}

		/**
		* Sets margin in percentage size of the inner box
		*/
		void SetMargin(float margin) {
			this->margin = margin;
		}

		/**
		* Recalculates size of this bounding box according to children of
		* the selected node
		*/
		void Recalc(Node* owner);

		/**
		* Gets bounding box rectangle
		*/
		ofRectangle GetBoundingBox() const {
			return boundingBox;
		}

	};
}// namespace