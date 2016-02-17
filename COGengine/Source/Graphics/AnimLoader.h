#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"

namespace Cog {

	typedef std::function<CommonAnim*()> SheetAnimBuilder;


	/**
	* XML animation loader
	*/
	class AnimLoader  {

	public:



		/**
		* Loads animation from xml
		*/
		void LoadAnimationsFromXml(spt<ofxXml> xml, vector<spt<CommonAnim>>& rootAnims, SheetAnimBuilder builder) {

				int numberOfAnims = xml->getNumTags("anim");
				// referenced anims will be processed at second phase
				map<int, CommonAnim*> referencedAnims;

				int actualAnimIndex = 0;
				// phase 1: load not-referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					// load
					spt<CommonAnim> anim = spt<CommonAnim>(CreateAnimationFromXml(xml, referencedAnims, actualAnimIndex, builder));
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
		CommonAnim* CreateAnimationFromXml(spt<ofxXml> xml, map<int, CommonAnim*>& referencedAnims, int& actualAnimIndex, SheetAnimBuilder builder) {

			int innerAnimations = xml->getNumTags("anim");

			CommonAnim* anim = builder();

			// fill ref and name attribute
			anim->LoadBaseAttributesFromXml(xml);

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
					spt<CommonAnim> newAnim = spt<CommonAnim>(CreateAnimationFromXml(xml, referencedAnims, ++actualAnimIndex, builder));
					anim->GetChildren().push_back(newAnim);
					xml->popTag();
				}
			}

			// set other attributes
			anim->LoadAttributesFromXml(xml);

			return anim;
		}


		/**
		* Finds animation by name, in selected collection
		*/
		spt<CommonAnim> FindAnimByName(string name, vector<spt<CommonAnim>> anims) {
			for (auto it = anims.begin(); it != anims.end(); ++it) {
				spt<CommonAnim> anim = (*it);
				if (anim->GetName() == name) return anim;
			}

			return spt<CommonAnim>();
		}

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of root for actual XML scope
		* @rootAnims list of all root animations
		*/
		void ProcessRefAnimationFromXml(spt<ofxXml> xml, map<int, CommonAnim*>& referencedAnims,
			int rootAnimIndex, vector<spt<CommonAnim>>& rootAnims, int& actualAnimIndex) {

			int innerAnimations = xml->getNumTags("anim");


			string ref = (xml->getAttribute(":", "ref", ""));
			string name = (xml->getAttribute(":", "name", ""));

			if (ref.length() != 0) {
				// got referenced animation

				CommonAnim* refAnim = referencedAnims[actualAnimIndex];

				if (ref.find(".") == -1) {
					// reference doesn't contain dot - it means that we can find its reference in THIS scope
					spt<CommonAnim> reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference != spt<CommonAnim>()) {
						// found reference
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference anim doesn't contain dot, but it isn't in this scope... that means it must refer to some other root animation
						spt<CommonAnim> rootReference = FindAnimByName(ref, rootAnims);
						if (rootReference == spt<CommonAnim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));
						refAnim->GetParametersFromReference(rootReference);
					}
				}
				else {
					// reference contains dot -> referenced animation should be in another scope
					string rootAnimName = ref.substr(0, ref.find("."));
					string subAnim = ref.substr(ref.find(".") + 1);
					spt<CommonAnim> root = FindAnimByName(rootAnimName, rootAnims);
					spt<CommonAnim> scopeAnim = root->FindChild(subAnim);
					if (root == spt<CommonAnim>() || scopeAnim == spt<CommonAnim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

					refAnim->GetParametersFromReference(scopeAnim);
				}

				// fill remaining attributes from XML
				refAnim->LoadAttributesFromXml(xml);

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