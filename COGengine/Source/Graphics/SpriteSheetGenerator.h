#include "ofMain.h"

using namespace std;

namespace Cog {

	class SpriteSheetGenerator {
		vector<string> images;

	public:
		void AddImage(string path) {
			this->images.push_back(path);
		}

		void GenerateSheet(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, string outputPath) {

			ofFbo fbo = ofFbo();
			fbo.allocate(sheetWidth, sheetHeight, GL_RGBA);
			fbo.begin();
			ofClear(0, 0, 0, 0);
			ofSetColor(255);

			int actualFrame = 0;
			for (auto image : images) {
				DrawImage(image, spriteWidth, spriteHeight, sheetWidth, sheetHeight, actualFrame);
			}

			fbo.end();
			fbo.draw(0, 0);
			ofPixels pixels = ofPixels();
			fbo.readToPixels(pixels);
			ofSaveImage(pixels, outputPath);
		}

	private:
		void DrawImage(string path, int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, int& actualFrame) {
			ofImage image = ofImage();
			image.load(path);
			int width = image.getWidth();
			int height = image.getHeight();

			int partsX = width / spriteWidth;
			int partsY = height / spriteHeight;

			if (width < spriteWidth && height < spriteHeight) {
				// draw centered
				int column = (actualFrame) % (sheetWidth / spriteWidth);
				int row = (actualFrame) / (sheetWidth / spriteWidth);
				int posX = 0;
				int posY = 0;

				//if (width > height) {
				//	posX = column*spriteWidth + spriteWidth / 2 - height / 2;
				//	posY = row*spriteHeight + spriteHeight / 2 - height / 2;
				//}
				//else{
					// always center by width
					posX = column*spriteWidth + spriteWidth / 2 - width / 2;
					posY = row*spriteHeight + spriteHeight / 2 - width / 2;
				//}
				
				image.drawSubsection(posX, posY,width, height, 0, 0, width, height);
				actualFrame++;
			}
			else {
				for (int i = 0; i < partsY; i++) {
					for (int j = 0; j < partsX; j++) {

						int column = (actualFrame) % (sheetWidth/spriteWidth);
						int row = (actualFrame) / (sheetWidth/spriteWidth);
						int posX = column*spriteWidth;
						int posY = row*spriteHeight;

						image.drawSubsection(posX, posY, spriteWidth, spriteHeight, j*spriteWidth, i*spriteHeight, spriteWidth, spriteHeight);

						actualFrame++;
					}
				}
			}
		}
	};

}