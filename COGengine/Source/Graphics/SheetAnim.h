#pragma once

#include "ofxCogCommon.h"
#include "CommonAnim.h"

namespace Cog {

	/**
	* Entity for discrete sheet animations
	*/
	class SheetAnim : public CommonAnim {
	protected:
		// path to sheet(s)
		string sheetPath = "";
		// number of frames per one line in spritesheet
		int frames = 1;
		// number of lines in spritesheet
		int lines = 1;
		// start frame
		int start = 1;
		// end frame
		int end = 1;
		// sequence increment
		int increment = 1;
		// list of paths to sheets this animation holds
		vector<string> sheets;
		// indicator, if sheet array has been initialized
		bool sheetInit = false;

	public:

		SheetAnim() {
		}

		SheetAnim(string name, string sheetPath, string ref, int frames, int lines, int start,
			int end, int increment, double speed, int repeat, bool isRevert) :
			CommonAnim(name, ref, speed, repeat, isRevert), sheetPath(sheetPath),
			frames(frames), lines(lines), start(start), end(end), increment(increment) {
		}


		/**
		* Copies all parameters from other animation
		*/
		void GetParametersFromReference(spt<CommonAnim> reference) {
			auto sheetAnimRef = static_pointer_cast<SheetAnim>(reference);

			this->SetSheetPath(sheetAnimRef->GetSheetPath());
			this->SetFrames(sheetAnimRef->GetFrames());
			this->SetLines(sheetAnimRef->GetLines());
			this->SetStart(sheetAnimRef->GetStart());
			this->SetEnd(sheetAnimRef->GetEnd());
			this->SetIncrement(sheetAnimRef->GetIncrement());

			CommonAnim::GetParametersFromReference(reference);
		}

		virtual void LoadAttributesFromXml(spt<ofxXml> xml) {
			this->SetSheetPath(xml->getAttribute(":", "sheet", this->GetSheetPath()));
			this->SetFrames(xml->getAttribute(":", "frames", this->GetFrames()));
			if (this->GetFrames() < 0) throw IllegalArgumentException(string_format("Error in animation %s; frames bust be greater or equal to 0", this->GetName().c_str()));
			this->SetLines(xml->getAttribute(":", "lines", this->GetLines()));
			if (this->GetLines() < 0) throw IllegalArgumentException(string_format("Error in animation %s; lines bust be greater or equal to 0", this->GetName().c_str()));
			this->SetStart(xml->getAttribute(":", "start", this->GetStart()));
			if (this->GetStart() < 0) throw IllegalArgumentException(string_format("Error in animation %s; start bust be greater or equal to 0", this->GetName().c_str()));
			this->SetEnd(xml->getAttribute(":", "end", this->GetEnd()));
			if (this->GetEnd() < 0) throw IllegalArgumentException(string_format("Error in animation %s; end bust be greater or equal to 0", this->GetName().c_str()));
			if (this->GetStart() > this->GetEnd()) throw IllegalArgumentException(string_format("Error in animation %s; start frame must be lower or equal to end frame", this->GetName().c_str()));
			this->SetIncrement(xml->getAttribute(":", "increment", this->GetIncrement()));
			if (this->GetIncrement() <= 0) throw IllegalArgumentException(string_format("Error in animation %s; increment must be greater than 0", this->GetName().c_str()));


			CommonAnim::LoadAttributesFromXml(xml);
		}


		/**
		* Initializes list of paths to all sheets this animation holds
		*/
		void InitSheets() {

			sheets.clear();

			if (!HasSheets()) {
				sheetInit = true;
				return;
			}

			// the file path can contain more sheets - in such case they are identified by braces, e.g. anim_frames_{0}.png or anim_frames_{000}.png
			int firstBracket = sheetPath.find("{");
			int secondBracket = sheetPath.find("}");


			if (firstBracket != string::npos && secondBracket != string::npos) {
				string sequencePrefix = sheetPath.substr(0, firstBracket);
				string sequenceSuffix = sheetPath.substr(secondBracket + 1, sheetPath.length() - (secondBracket + 1));

				int numberOfDigits = secondBracket - firstBracket - 1;

				if (start > end) throw IllegalArgumentException("Start of the animation must be lower or equal to the end");

				// parse sheet files
				for (int i = start; i <= end; i += increment) {
					string file = sequencePrefix + ofToString(i, 1, numberOfDigits, '0') + sequenceSuffix;
					sheets.push_back(file);
				}
			}
			else {
				// just store path to the sheet
				sheets.push_back(sheetPath);
			}

			sheetInit = true;
		}

		/**
		* Get sheets at selected index
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
		* Gets number of frames per one row in spritesheet
		*/
		const int GetFrames() const {
			return frames;
		}

		/**
		* Gets total number of frames
		*/
		const int GetTotalFrames() const {
			return (end - start + 1 + (increment - 1)) / increment;
		}

		/**
		* Sets number of frames per one row in spritesheet
		*/
		void SetFrames(int frames) {
			this->frames = frames;
		}

		/**
		* Gets number of lines in spritesheet
		*/
		const int GetLines() const {
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
		const int GetStart() const {
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
		const int GetEnd() const {
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
		const int GetIncrement() const {
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