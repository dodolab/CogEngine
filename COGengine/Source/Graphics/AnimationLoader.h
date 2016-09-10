#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "Anim.h"

namespace Cog {

	/**
	* XML animation loader
	*/
	class AnimationLoader  {

	public:

		/**
		* Loads animation from xml
		*/
		void LoadAnimations(spt<ofxXmlSettings> xml, vector<spt<Anim>>& rootAnims) {

				int numberOfAnims = xml->getNumTags("anim");
				// referenced anims will be processed at second phase
				map<string, Anim*> referencedAnims;

				// phase 1: load not-referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					// load
					spt<Anim> anim = spt<Anim>(CreateAnimationFromXml(xml, referencedAnims));
					if (anim->GetName().length() == 0) throw ConfigErrorException("All root animations must have a name!");

					rootAnims.push_back(anim);
					xml->popTag();
				}

				// phase 2: load referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					ProcessRefAnimationFromXml(xml, referencedAnims, i, rootAnims);
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
		Anim* CreateAnimationFromXml(spt<ofxXmlSettings> xml, map<string, Anim*>& referencedAnims) {

			int innerAnimations = xml->getNumTags("anim");

			Anim* anim = new Anim();

			// fill ref and name attribute
			FillBaseAttributes(xml, anim);

			if (anim->GetRef().length() != 0) {
				// animation is referenced -> push it to the referencedAnims and return
				referencedAnims[anim->GetName()] = anim;
				if (innerAnimations != 0) throw ConfigErrorException("Referenced animations mustn't have inner animations!");

				return anim;
			}
			else {
				// children will be taken from referenced animation
				for (int i = 0; i < innerAnimations; i++) {
					xml->pushTag("anim", i);
					spt<Anim> newAnim = spt<Anim>(CreateAnimationFromXml(xml, referencedAnims));
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
		void FillBaseAttributes(spt<ofxXmlSettings> xml, Anim* anim) {
			anim->SetRef(xml->getAttribute(":", "ref", anim->GetRef()));
			anim->SetName(xml->getAttribute(":", "name", anim->GetName()));
		}

		/**
		* Fills other attributes
		*/
		void FillOtherAttributes(spt<ofxXmlSettings> xml, Anim* anim) {
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
		spt<Anim> FindAnimByName(string name, vector<spt<Anim>> anims) {
			for (auto it = anims.begin(); it != anims.end(); ++it) {
				spt<Anim> anim = (*it);
				if (anim->GetName() == name) return anim;
			}


			return spt<Anim>();
		}

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of root for actual XML scope
		* @rootAnims list of all root animations
		*/
		void ProcessRefAnimationFromXml(spt<ofxXmlSettings> xml, map<string, Anim*>& referencedAnims,
			int rootAnimIndex, vector<spt<Anim>>& rootAnims) {

			int innerAnimations = xml->getNumTags("anim");


			string ref = (xml->getAttribute(":", "ref", ""));
			string name = (xml->getAttribute(":", "name", ""));

			if (ref.length() != 0) {
				// got referenced animation

				Anim* refAnim = referencedAnims[name];

				if (ref.find(".") == -1) {
					// reference doesn't contain dot - it means that we can find its reference in THIS scope
					spt<Anim> reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference != spt<Anim>()) {
						// found reference
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference anim doesn't contain dot, but it isn't in this scope... that means it must refer to some other root animation
						spt<Anim> rootReference = FindAnimByName(ref, rootAnims);
						if (rootReference == spt<Anim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));
						refAnim->GetParametersFromReference(rootReference);
					}
				}
				else {
					// reference contains dot -> referenced animation should be in another scope
					string rootAnimName = ref.substr(0, ref.find("."));
					string subAnim = ref.substr(ref.find(".") + 1);
					spt<Anim> root = FindAnimByName(rootAnimName, rootAnims);
					spt<Anim> scopeAnim = root->FindChild(subAnim);
					if (root == spt<Anim>() || scopeAnim == spt<Anim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

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
					ProcessRefAnimationFromXml(xml, referencedAnims, rootAnimIndex, rootAnims);
					xml->popTag();
				}
			}
		}

	};

} // namespace