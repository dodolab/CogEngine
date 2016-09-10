#pragma once

#include "Definitions.h"
#include "Settings.h"
#include "Tile.h"

namespace Cog {

	class Sprite;
	class SpriteSheet;

	/**
	* Tile map loader
	*/
	class MapLoader {

	public:

		/**
		* Loads list of tiles from text file
		* Text file must be in this format:
		*
		* x x x x
		* x x x x
		* x x x x
		* where x is tile identifier
		*
		* @param filename path to file
		* @param idNameMap map with identifier->human-readable-name, can be empty
		* @param map output map
		*/
		void LoadFromTextFile(string filename, Setting& idNameMap, TileMap& map);

		/**
		* Loads list of tiles from image
		* Each color will represent different type of tile
		* 
		* @param filename path to file
		* @param idNameMap map with identifier->human-readable-name, can be empty
		* @param map output map
		*/
		void LoadFromPNGImage(string filename, Setting& idNameMap, TileMap& map);

		/**
		* Generates a nice map image from tiles and sprites
		* @param tiles tile map
		* @param sprites collection of sprites indexed by tiles
		* @param spriteSheet sprite sheet with all data needed to render
		* @param filePath output file path
		* @param scale scale of the generated image
		*/
		void GenerateMapImage(TileMap& tiles, map<string, vector<Sprite>>& sprites, 
			spt<SpriteSheet> spriteSheet, string filePath, float scale);

	};

} // namespace