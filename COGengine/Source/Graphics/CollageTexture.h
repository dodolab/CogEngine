#pragma once

#include "ofMain.h"

namespace Cog {

	/**x
	Collage Texture provides simple methods for allocating and filling a texture
	with the contents of arbitrary files. Seperate images
	could be combined into a single sprite sheet on the fly, but it could be
	used for any purpose. It handles alpha blending of pasted images as well.
	*/
	class CollageTexture : public ofTexture
	{

	public:

		~CollageTexture();

		/**
		* Allocates memory for collage texture
		* @param w total width
		* @param h total height
		* @param internalGlDataType type of textures (GL_RGBA or GL_RGB)
		* @param internalGLScaleMode scale mode (GL_LINEAR or GL_NEAREST)
		*/
		void allocate(int w, int h, int internalGlDataType, int internalGLScaleMode); // scale mode is GL_LINEAR or GL_NEAREST if you want pixel scaling

		/**
		* Allocates memory for collage texture
		* @param w total width
		* @param h total height
		* @param internalGlDataType type of textures (GL_RGBA or GL_RGB)
		* @param internalGLScaleMode scale mode (GL_LINEAR or GL_NEAREST)
		* @param bUseARBExtension if true, ARB extension will be used
		*/
		void allocate(int w, int h, int internalGlDataType, int internalGLScaleMode, bool bUseARBExtention);

		/**
		* Pastes an image into texture
		* @param x left offset in pixels
		* @param y top offset in pixels
		* @param texturePath path to the texture
		* @param glType type of texture (GL_RGBA or GL_RGB)
		*/
		void pasteImage(int x, int y, string texturePath, int glType = GL_RGBA);

		void pasteImage(int x, int y, int w, int h, ofImage* image);

		/**
		* Pastes an image into texture
		* @param x left offset in pixels
		* @param y top offset in pixels
		* @param w texture width in pixels
		* @param h texture height in pixels
		* @param pxls pixel array
		* @param glType type of texture (GL_RGBA or GL_RGB)
		*/
		void pasteImage(int x, int y, int w, int h, unsigned char * pxls, int glType = GL_RGBA);


		/**
		* Finishes pasting images and creates a texture
		*/
		void finish();

		// pixel array
		unsigned char * collage;
		int c_width;
		int c_height;
		int c_bpp;
		int c_type;

	protected:

		struct {
			GLenum glType;
			GLenum pixelType;
		} internal;

		/**
		* Gets 1D index by texture position
		*/
		int getIndex(int x, int y);
	};

}// namespace