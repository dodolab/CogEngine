#include "SheetAnim.h"
#include "Utils.h"
#include "Error.h"
#include "Facade.h"

namespace Cog {

	SheetAnim::SheetAnim(string name, string sheetPath, string ref, int frames, int lines, int start,
		int end, int increment, float speed, int repeat, bool isRevert) :
		GeneralAnim(name, ref, speed, repeat, isRevert), sheetPath(sheetPath),
		frames(frames), lines(lines), start(start), end(end), increment(increment) {
	}

	void SheetAnim::GetParametersFromReference(spt<GeneralAnim> reference) {
		auto sheetAnimRef = static_pointer_cast<SheetAnim>(reference);

		this->SetSheetPath(sheetAnimRef->GetSheetPath());
		this->SetFrames(sheetAnimRef->GetFrames());
		this->SetLines(sheetAnimRef->GetLines());
		this->SetStart(sheetAnimRef->GetStart());
		this->SetEnd(sheetAnimRef->GetEnd());
		this->SetIncrement(sheetAnimRef->GetIncrement());

		GeneralAnim::GetParametersFromReference(reference);
	}


	void SheetAnim::LoadAttributesFromXml(xml_node& xml) {

		// check all values
		this->SetSheetPath(xml.attribute("sheet").as_string(this->GetSheetPath().c_str()));
		this->SetFrames(xml.attribute("frames").as_int(this->GetFrames()));
		if (this->GetFrames() < 0) throw IllegalArgumentException(string_format("Error in animation %s; frames bust be greater or equal to 0", this->GetName().c_str()));
		this->SetLines(xml.attribute("lines").as_int(this->GetLines()));
		if (this->GetLines() < 0) throw IllegalArgumentException(string_format("Error in animation %s; lines bust be greater or equal to 0", this->GetName().c_str()));
		this->SetStart(xml.attribute("start").as_int(this->GetStart()));
		if (this->GetStart() < 0) throw IllegalArgumentException(string_format("Error in animation %s; start bust be greater or equal to 0", this->GetName().c_str()));
		this->SetEnd(xml.attribute("end").as_int(this->GetEnd()));
		if (this->GetEnd() < 0) throw IllegalArgumentException(string_format("Error in animation %s; end bust be greater or equal to 0", this->GetName().c_str()));
		if (this->GetStart() > this->GetEnd()) throw IllegalArgumentException(string_format("Error in animation %s; start frame must be lower or equal to end frame", this->GetName().c_str()));
		this->SetIncrement(xml.attribute("increment").as_int(this->GetIncrement()));
		if (this->GetIncrement() <= 0) throw IllegalArgumentException(string_format("Error in animation %s; increment must be greater than 0", this->GetName().c_str()));

		// load other attributes
		GeneralAnim::LoadAttributesFromXml(xml);
	}

	void SheetAnim::InitSheets() {

		sheets.clear();

		if (!HasSheets()) {
			sheetInit = true;
			return;
		}

		// the file path can contain more sheets - in this case they are identified by braces
		// e.g. anim_frames_{0}.png or anim_frames_{000}.png
		
		int firstBracket = sheetPath.find("{");
		int secondBracket = sheetPath.find("}");


		if (firstBracket != string::npos && secondBracket != string::npos) {
			string sequencePrefix = sheetPath.substr(0, firstBracket);
			string sequenceSuffix = sheetPath.substr(secondBracket + 1, sheetPath.length() - (secondBracket + 1));

			int numberOfDigits = secondBracket - firstBracket - 1;

			if (start > end) throw IllegalArgumentException("Start frame of the animation must be lower or equal to the final frame");

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

	void SheetAnim::PreloadImages() {
		auto& sheetPaths = GetSheetPaths();
		// preload images
		for (auto sheet : sheetPaths) {
			CogGet2DImage(sheet);
		}
	}

	spt<GeneralAnim> SheetAnim::Clone() {
		auto output = spt<SheetAnim>(new SheetAnim(*this));
		return output;
	}

} // namespace