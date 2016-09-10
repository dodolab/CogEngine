#pragma once

#include "ofVec2f.h"

namespace Cog {

	/**
	* Two-integer vector
	*/
	struct Vec2i {

		Vec2i() : x(0), y(0) {

		}

		Vec2i(int xy) : x(xy), y(xy) {

		}

		Vec2i(int x, int y) : x(x), y(y) {

		}

		/** position on the x axis */
		int x;
		/** position on the y axis */
		int y;


		inline operator size_t() const {
			// we use seed for Mersenne Twister generator
			return x * 1812433253 + y;
		}

		inline operator ofVec2f() const {
			return ofVec2f(float(x), (float)y);
		}

		inline bool operator==(const Vec2i& a) const {
			return x == a.x && y == a.y;
		}

		inline bool operator!=(const Vec2i& a) const {
			return x != a.x || y != a.y;
		}

		inline bool operator<(const Vec2i& a) const {
			return ((size_t)*this) < ((size_t)a);
		}

		inline bool operator>(const Vec2i& a) const {
			return ((size_t)*this) > ((size_t)a);
		}

		inline Vec2i operator+(const Vec2i& a) const {
			return Vec2i(a.x + x, a.y + y);
		}

		inline Vec2i operator-(const Vec2i& a) const {
			return Vec2i(x - a.x, y - a.y);
		}
	};
}
