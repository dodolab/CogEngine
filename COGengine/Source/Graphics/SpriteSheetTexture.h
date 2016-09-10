#pragma once

#include "ofTexture.h"

namespace Cog {

	/**
	* Texture with the contents of various files
	* Can consist of many images, combined into a single sprite texture
	*/
	class SpriteSheetTexture : public ofTexture
	{

	public:

		~SpriteSheetTexture();

		/**
		* Allocates memory
		* @param w texture width
		* @param h texture height
		* @param glType type of texture (GL_RGBA or GL_RGB)
		* @param glScaleMode scaling mode (GL_LINEAR or GL_NEAREST)
		*/
		virtual void allocate(int w, int h, int glType, int glScaleMode); 

		/**
		* Allocates memory
		* @param w texture width
		* @param h texture height
		* @param glType type of texture (GL_RGBA or GL_RGB)
		* @param glScaleMode scaling mode (GL_LINEAR or GL_NEAREST)
		* @param useARB if true, ARB extension will be used (ARB is a rectangle texture without mipmaps)
		*/
		virtual void allocate(int w, int h, int glType, int glScaleMode, bool useARB);

		/**
		* Pastes an image into texture
		* @param x left offset in pixels
		* @param y top offset in pixels
		* @param texturePath path to the texture
		* @param glType type of texture (GL_RGBA or GL_RGB)
		*/
		void PasteImage(int x, int y, string texturePath, int glType = GL_RGBA);

		/**
		* Pastes an image into texture
		* @param x left offset in pixels
		* @param y top offset in pixels
		* @param w texture width
		* @param h texture height
		* @param image image to paste
		*/
		void PasteImage(int x, int y, int w, int h, ofImage* image);

		/**
		* Pastes an image into texture
		* @param x left offset in pixels
		* @param y top offset in pixels
		* @param w texture width
		* @param h texture height
		* @param pxls pixel data
		* @param glType type of texture (GL_RGBA or GL_RGB)
		*/
		void PasteImage(int x, int y, int w, int h, unsigned char * pxls, int glType = GL_RGBA);


		/**
		* Finishes pasting images and creates a texture
		*/
		void Finish();

		unsigned char * pixels;
		int width;
		int height;
		int channels; // number of color channels
		int glType;

	protected:

		/**
		* Gets 1D index by texture position
		*/
		int GetIndex(int x, int y);
	};

}// namespace