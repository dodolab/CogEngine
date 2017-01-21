
#include "SpriteSheetTexture.h"
#include "ofImage.h"

namespace Cog {

	SpriteSheetTexture::~SpriteSheetTexture()
	{
		delete[] pixels;
	}

	void SpriteSheetTexture::allocate(int w, int h, int glType, int glScaleMode) {
		allocate(w, h, glType, glScaleMode, ofGetUsingArbTex());
	}

	void SpriteSheetTexture::allocate(int w, int h, int glType, int glScaleMode, bool useARB) {

		// allocate pixel data
		if (glType == GL_RGB)
			pixels = new unsigned char[w*h * 3];
		else if (glType == GL_RGBA)
			pixels = new unsigned char[w*h * 4];

		this->width = w;
		this->height = h;

		if (glType == GL_RGB)
			this->channels = 3;
		else if (glType == GL_RGBA)
			this->channels = 4;

		// set zero to all channels
		for (int i = 0; i < w*h*channels; i++)
		{
			pixels[i] = 0;
		}

		this->glType = glType;

#ifndef TARGET_OPENGLES
		if (useARB && GL_ARB_texture_rectangle) {
			// ARB is for custom rectangle texture without mipmaps
			texData.tex_w = w;
			texData.tex_h = h;
			texData.tex_t = w;
			texData.tex_u = h;
			texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
		}
		else
#endif
		{
			//calculate next power of 2
			texData.tex_w = ofNextPow2(w);
			texData.tex_h = ofNextPow2(h);
			texData.tex_t = 1.0f;
			texData.tex_u = 1.0f;
			texData.textureTarget = GL_TEXTURE_2D;
		}

		texData.glInternalFormat = glType;

		int internalGlType = 0;
		int internalPixelType = 0;

		switch (texData.glInternalFormat) {
#ifndef TARGET_OPENGLES	
		case GL_RGBA32F_ARB:
		case GL_RGBA16F_ARB:
			internalGlType = GL_RGBA;
			internalPixelType = GL_FLOAT;
			break;

		case GL_RGB32F_ARB:
			internalGlType = GL_RGB;
			internalPixelType = GL_FLOAT;
			break;

		case GL_LUMINANCE32F_ARB:
			internalGlType = GL_LUMINANCE;
			internalPixelType = GL_FLOAT;
			break;
#endif			

		default:
			internalGlType = GL_LUMINANCE;
			internalPixelType = GL_UNSIGNED_BYTE;
		}

		// free previous texture
		clear();

		glGenTextures(1, (GLuint *)&texData.textureID);
		glEnable(texData.textureTarget);

		// set parameters and draw image
		glBindTexture(texData.textureTarget, (GLuint)texData.textureID);

		glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, glScaleMode);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, glScaleMode);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, glScaleMode);
		glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, glScaleMode);

#ifndef TARGET_OPENGLES
		// for OPENGL_ES, glType and glDataType must be the same
		glTexImage2D(texData.textureTarget, 0, texData.glInternalFormat, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, internalGlType, internalPixelType, 0);
#else
		glTexImage2D(texData.textureTarget, 0, texData.glInternalFormat, texData.tex_w, texData.tex_h, 0, texData.glInternalFormat, GL_UNSIGNED_BYTE, 0);
#endif

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glDisable(texData.textureTarget);

		texData.width = w;
		texData.height = h;
		texData.bFlipTexture = false;
		texData.bAllocated = true;
	}

	void SpriteSheetTexture::PasteImage(int x, int y, string texturePath, int glType)
	{
		ofImage loader;
		loader.setUseTexture(false);
		loader.loadImage(texturePath);
		PasteImage(x, y, loader.getWidth(), loader.getHeight(), loader.getPixels().getData(), glType);
		loader.clear();
	}

	void SpriteSheetTexture::PasteImage(int x, int y, int w, int h, ofImage* image) {
		PasteImage(x, y, w, h, image->getPixels().getData(), this->texData.glInternalFormat);
	}

	void SpriteSheetTexture::PasteImage(int x, int y, int w, int h, unsigned char * pxls, int glType)
	{
		int bpp;

		if (glType == GL_RGB)
			bpp = 3;
		else if (glType == GL_RGBA)
			bpp = 4;


		int pixelsIndex = GetIndex(x, y);
		int sourceIndex = 0;
		int sourceRow = 0;

		int collageMax = width*height*channels;
		int sourceMax = w*h*bpp;

		int startX = x;

		while (1)
		{
			// draw rows one by one
			if (x >= width || x - startX >= w)
			{
				x = startX;
				y++;
				sourceRow++;
				sourceIndex = (sourceRow * w)*bpp;
				pixelsIndex = GetIndex(x, y);

				// passed the edge of the texture
				if (pixelsIndex >= collageMax)
					break;
			}

			if (sourceIndex >= sourceMax) break;

			if (channels == 4)
			{
				// blend alpha channels

				if (pixels[pixelsIndex + 3] != 0)
				{
					// something non transparent exists as this pixel
					float dest_pct = (float)pixels[pixelsIndex + 3] / 255.0f;

					float source_pct = 1;
					if (bpp == 4)
						source_pct = (float)pxls[sourceIndex + 3] / 255.0f;

					//additive blend
					pixels[pixelsIndex] = (float)pixels[pixelsIndex] * (1.0 - source_pct) + (float)pxls[sourceIndex] * source_pct;
					pixels[pixelsIndex + 1] = (float)pixels[pixelsIndex + 1] * (1.0 - source_pct) + (float)pxls[sourceIndex + 1] * source_pct;
					pixels[pixelsIndex + 2] = (float)pixels[pixelsIndex + 2] * (1.0 - source_pct) + (float)pxls[sourceIndex + 2] * source_pct;

					pixels[pixelsIndex + 3] = 255 * (dest_pct * (1.0 - source_pct) + source_pct);
				}
				else 
				{
					// this pixel is transparent
					pixels[pixelsIndex] = pxls[sourceIndex];
					pixels[pixelsIndex + 1] = pxls[sourceIndex + 1];
					pixels[pixelsIndex + 2] = pxls[sourceIndex + 2];
					if (bpp == 4)
						pixels[pixelsIndex + 3] = pxls[sourceIndex + 3];
					else
						pixels[pixelsIndex + 3] = 255;
				}

			}
			else 
			{
				// no blend, just copy pixels
				pixels[pixelsIndex] = pxls[sourceIndex];
				pixels[pixelsIndex + 1] = pxls[sourceIndex + 1];
				pixels[pixelsIndex + 2] = pxls[sourceIndex + 2];
			}

			x++;
			pixelsIndex += channels;
			sourceIndex += channels;
		}
	}

	void SpriteSheetTexture::Finish()
	{
		loadData(pixels, width, height, glType);

		// reset data
		for (int i = 0; i < width*height*channels; i++)
			pixels[i] = 0;
	}

	int SpriteSheetTexture::GetIndex(int x, int y)
	{
		return (y*width + x)*channels;
	}

}// namespace