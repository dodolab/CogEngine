#pragma once


namespace Cog {

	class Node;

	class HitBox  {
	private:
		float posX;
		float posY;
		float width = 1;
		float height = 1;

	public:
		HitBox() {
		}

		void Recalc(Node* owner);

		float GetWidth() {
			return (float)width;
		}

		float GetHeight() {
			return (float)height;
		}

		float GetPosX() {
			return posX;
		}

		float GetPosY() {
			return posY;
		}
	};

}// namespace