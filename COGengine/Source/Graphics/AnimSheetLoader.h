#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"

namespace Cog {

	/**
	* XML animation loader
	*/
	class AnimSheetLoader  {

	public:

		/**
		* Loads animation from xml
		*/
		void LoadAnimationsFromXml(spt<ofxXml> xml, vector<spt<SheetAnim>>& rootAnims) {

				int numberOfAnims = xml->getNumTags("anim");
				// referenced anims will be processed at second phase
				map<int, SheetAnim*> referencedAnims;

				int actualAnimIndex = 0;
				// phase 1: load not-referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					// load
					spt<SheetAnim> anim = spt<SheetAnim>(CreateAnimationFromXml(xml, referencedAnims, actualAnimIndex));
					if (anim->GetName().length() == 0) throw ConfigErrorException("All root animations must have a name!");

					rootAnims.push_back(anim);
					xml->popTag();
				}

				actualAnimIndex = 0;
				// phase 2: load referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					ProcessRefAnimationFromXml(xml, referencedAnims, i, rootAnims, actualAnimIndex);
					xml->popTag();
				}
		}

	private:

		/**
		* Creates animation from XML
		* @param xml xml to load from
		* @param referencedAnims array that will be filled with animations that couldn't be load yet becase
		* they reference to another animation
		*/
		SheetAnim* CreateAnimationFromXml(spt<ofxXml> xml, map<int, SheetAnim*>& referencedAnims, int& actualAnimIndex) {

			int innerAnimations = xml->getNumTags("anim");

			SheetAnim* anim = new SheetAnim();

			// fill ref and name attribute
			FillBaseAttributes(xml, anim);

			if (anim->GetRef().length() != 0) {
				// animation is referenced -> push it to the referencedAnims and return
				referencedAnims[actualAnimIndex] = anim;
				if (innerAnimations != 0) throw ConfigErrorException("Referenced animations mustn't have inner animations!");

				return anim;
			}
			else {
				// children will be taken from referenced animation
				for (int i = 0; i < innerAnimations; i++) {
					xml->pushTag("anim", i);
					spt<SheetAnim> newAnim = spt<SheetAnim>(CreateAnimationFromXml(xml, referencedAnims, ++actualAnimIndex));
					anim->GetChildren().push_back(newAnim);
					xml->popTag();
				}
			}

			// set other attributes
			FillOtherAttributes(xml, anim);

			return anim;
		}

		/**
		* Fills base attributes (ref and name)
		*/
		void FillBaseAttributes(spt<ofxXml> xml, SheetAnim* anim) {
			anim->SetRef(xml->getAttribute(":", "ref", anim->GetRef()));
			anim->SetName(xml->getAttribute(":", "name", anim->GetName()));
		}

		/**
		* Fills other attributes
		*/
		void FillOtherAttributes(spt<ofxXml> xml, SheetAnim* anim) {
			anim->SetSheetPath(xml->getAttribute(":", "sheet", anim->GetSheetPath()));
			anim->SetFrames(xml->getAttribute(":", "frames", anim->GetFrames()));
			if (anim->GetFrames() < 0) throw IllegalArgumentException(string_format("Error in animation %s; frames bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetLines(xml->getAttribute(":", "lines", anim->GetLines()));
			if (anim->GetLines() < 0) throw IllegalArgumentException(string_format("Error in animation %s; lines bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetStart(xml->getAttribute(":", "start", anim->GetStart()));
			if (anim->GetStart() < 0) throw IllegalArgumentException(string_format("Error in animation %s; start bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetEnd(xml->getAttribute(":", "end", anim->GetEnd()));
			if (anim->GetEnd() < 0) throw IllegalArgumentException(string_format("Error in animation %s; end bust be greater or equal to 0", anim->GetName().c_str()));
			if (anim->GetStart() > anim->GetEnd()) throw IllegalArgumentException(string_format("Error in animation %s; start frame must be lower or equal to end frame", anim->GetName().c_str()));
			anim->SetIncrement(xml->getAttribute(":", "increment", anim->GetIncrement()));
			if (anim->GetIncrement() <= 0) throw IllegalArgumentException(string_format("Error in animation %s; increment must be greater than 0", anim->GetName().c_str()));
			anim->SetSpeed(xml->getAttribute(":", "speed", anim->GetSpeed()));
			if (anim->GetSpeed() < 0) throw IllegalArgumentException(string_format("Error in animation %s; speed bust be greater than 0", anim->GetName().c_str()));
			anim->SetRepeat(xml->getAttribute(":", "repeat", anim->GetRepeat()));
			if (anim->GetRepeat() < 0) throw IllegalArgumentException(string_format("Error in animation %s; number of repetitions must be greater or equal to 0", anim->GetName().c_str()));
			anim->SetIsRevert(xml->getBoolAttribute(":", "revert", anim->GetIsRevert()));
		}

		/**
		* Finds animation by name, in selected collection
		*/
		spt<SheetAnim> FindAnimByName(string name, vector<spt<SheetAnim>> anims) {
			for (auto it = anims.begin(); it != anims.end(); ++it) {
				spt<SheetAnim> anim = (*it);
				if (anim->GetName() == name) return anim;
			}


			return spt<SheetAnim>();
		}

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of root for actual XML scope
		* @rootAnims list of all root animations
		*/
		void ProcessRefAnimationFromXml(spt<ofxXml> xml, map<int, SheetAnim*>& referencedAnims,
			int rootAnimIndex, vector<spt<SheetAnim>>& rootAnims, int& actualAnimIndex) {

			int innerAnimations = xml->getNumTags("anim");


			string ref = (xml->getAttribute(":", "ref", ""));
			string name = (xml->getAttribute(":", "name", ""));

			if (ref.length() != 0) {
				// got referenced animation

				SheetAnim* refAnim = referencedAnims[actualAnimIndex];

				if (ref.find(".") == -1) {
					// reference doesn't contain dot - it means that we can find its reference in THIS scope
					spt<SheetAnim> reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference != spt<SheetAnim>()) {
						// found reference
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference anim doesn't contain dot, but it isn't in this scope... that means it must refer to some other root animation
						spt<SheetAnim> rootReference = FindAnimByName(ref, rootAnims);
						if (rootReference == spt<SheetAnim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));
						refAnim->GetParametersFromReference(rootReference);
					}
				}
				else {
					// reference contains dot -> referenced animation should be in another scope
					string rootAnimName = ref.substr(0, ref.find("."));
					string subAnim = ref.substr(ref.find(".") + 1);
					spt<SheetAnim> root = FindAnimByName(rootAnimName, rootAnims);
					spt<SheetAnim> scopeAnim = root->FindChild(subAnim);
					if (root == spt<SheetAnim>() || scopeAnim == spt<SheetAnim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

					refAnim->GetParametersFromReference(scopeAnim);
				}

				// fill remaining attributes from XML
				FillOtherAttributes(xml, refAnim);

			}
			else {
				// this animation doesn't have ref attribute -> keep searching over its children

				for (int i = 0; i < innerAnimations; i++) {
					// use recursion
					xml->pushTag("anim", i);
					ProcessRefAnimationFromXml(xml, referencedAnims, rootAnimIndex, rootAnims, ++actualAnimIndex);
					xml->popTag();
				}
			}
		}

	};

} // namespace