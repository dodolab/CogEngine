#pragma once

#include "GeneralAnim.h"

namespace Cog {

	/**
	* Entity for sprite sheet animations
	*/
	class SheetAnim : public GeneralAnim {
	protected:
		// path to sheet
		string sheetPath = "";
		// number of frames per one line the sprite sheet contains
		int frames = 1;
		// number of lines in spritesheet
		int lines = 1;
		// start frame
		int start = 1;
		// final frame
		int end = 1;
		// sequence increment (e.g. value of 2 switches to new frame every second Draw loop)
		int increment = 1;
		// list of paths to sheets this animation contains
		vector<string> sheets;
		// indicator whether the sheet array has been initialized
		bool sheetInit = false;

	public:

		SheetAnim() {
		}

		/**
		* Creates a new sheet animation
		* @param name animation name
		* @param sheetPath path to sheet
		* @param ref name of referenced animation
		* @param frames number of frames per one line the sprite sheet contains
		* @param lines number of lines in sprite sheet
		* @param start starting fram,e
		* @param end final frame
		* @param increment sequence increment (e.g. value of 2 switches to new frame every second Draw loop)
		* @param speed animation speed
		* @param repeat number of repetitions (0 for infinite)
		* @param isRevert indicator whether the animation is inverted
		*/
		SheetAnim(string name, string sheetPath, string ref, int frames, int lines, int start,
			int end, int increment, float speed, int repeat, bool isRevert);


		/**
		* Copies all parameters from other animation
		*/
		void GetParametersFromReference(spt<GeneralAnim> reference);


		virtual void LoadAttributesFromXml(spt<ofxXml> xml);


		/**
		* Initializes list of paths to all sheets this animation contains
		*/
		void InitSheets();

		/**
		* Gets sheet name at selected index
		*/
		string GetSheet(int frameIndex) {
			if (!sheetInit) InitSheets();

			return sheets[frameIndex];
		}

		virtual bool IsAnimatable() {
			return HasSheets();
		}

		virtual bool IsContinous() {
			return false;
		}

		virtual int GetDuration() {
			return GetTotalFrames();
		}

		/**
		* Returns true, if this animation has sheet(s)
		*/
		bool HasSheets() {
			return sheetPath.length() != 0;
		}

		/**
		* Gets path to the animation sheet
		*/
		const string GetSheetPath() const {
			return sheetPath;
		}

		/**
		* Sets path to the animation sheet
		*/
		void SetSheetPath(string sheetPath) {
			this->sheetPath = sheetPath;
		}

		/**
		* Gets number of frames in one line in the sprite sheet
		*/
		int GetFrames() const {
			return frames;
		}

		/**
		* Gets total number of frames
		*/
		int GetTotalFrames() const {
			return (end - start + 1 + (increment - 1)) / increment;
		}

		/**
		* Sets number of frames in one line in the sprite sheet
		*/
		void SetFrames(int frames) {
			this->frames = frames;
		}

		/**
		* Gets number of lines in spritesheet
		*/
		int GetLines() const {
			return lines;
		}

		/**
		* Sets number of lines in spritesheet
		*/
		void SetLines(int lines) {
			this->lines = lines;
		}

		/**
		* Gets starting frame
		*/
		int GetStart() const {
			return start;
		}

		/**
		* Sets starting frame
		*/
		void SetStart(int start) {
			this->start = start;
		}

		/**
		* Gets ending frame
		*/
		int GetEnd() const {
			return end;
		}

		/**
		* Sets ending frame
		*/
		void SetEnd(int end) {
			this->end = end;
		}

		/**
		* Gets increment value
		*/
		int GetIncrement() const {
			return increment;
		}

		/**
		* Sets increment value
		*/
		void SetIncrement(int increment) {
			this->increment = increment;
		}

		/**
		* Gets collection of paths to all sheets
		*/
		vector<string>& GetSheetPaths() {
			if (!sheetInit) InitSheets();
			return sheets;
		}
	};



}// namespace