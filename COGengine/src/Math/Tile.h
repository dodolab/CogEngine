#pragma once

#include <vector>
#include <string>
using namespace std;

namespace Cog {

	/**
	* Tile used in grid-based graphs
	*/
	class Tile {
	public:
		// tile identifier
		int identifier;
		int index;
		string name;
		int posX;
		int posY;

		Tile(int identifier, int index, string name, int posX, int posY) 
			: identifier(identifier), index(index), name(name), posX(posX), posY(posY) {

		}
	};

	/**
	* Map of tiles
	*/
	class TileMap {
	public:
		vector<Tile> tiles;
		int width;
		int height;

		TileMap() {

		}

		TileMap(int width, int height) : width(width), height(height) {

		}

		Tile& GetTile(int x, int y) {
			return tiles[y*width + x];
		}
	};

} // namespace