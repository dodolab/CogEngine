#include <vector>
#include <string>
using namespace std;

namespace Cog {

	/**
	* Generator for sprite sheets 
	* (created for game architects that need to compose their frames into one sprite sheet)
	*/
	class SpriteSheetGenerator {
		vector<string> images;

	public:
		/**
		* Adds an image into collection
		*/
		void AddImage(string path) {
			this->images.push_back(path);
		}

		/**
		* Generates sprite sheet image
		* @param spriteWidth width of one sprite
		* @param spriteHeight height of one sprite
		* @param sheetWidth width of the whole sprite sheet
		* @param sheetHeight height of the whole sprite sheet
		* @param outputPath output image path
		*/
		void GenerateSheet(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, string outputPath);

	private:
		/**
		* Draws one image into FBO
		* @param path path to image
		* @param spriteWidth width of one sprite
		* @param spriteHeight height of one sprite
		* @param sheetWidth width of the whole sprite sheet
		* @param sheetHeight height of the whole sprite sheet
		* @param actualFrame output value - actual frame index being rendered
		*/
		void DrawImage(string path, int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, int& actualFrame);
	};

}