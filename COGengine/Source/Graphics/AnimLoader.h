#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"

namespace Cog {

	typedef std::function<CommonAnim*()> SheetAnimBuilder;


	/**
	* Class that loads animations from XML
	*/
	class AnimLoader  {

	public:

		/**
		* Loads animation from xml
		* @param xml xml to load from
		* @param rootAnims output collection that will contain all animations on the top of the tree
		* @param builder instance that creates new instances of the animation object
		*/
		void LoadAnimationsFromXml(spt<ofxXml> xml, vector<spt<CommonAnim>>& rootAnims, SheetAnimBuilder builder) {

				int numberOfAnims = xml->getNumTags("anim");

				// referenced anims (those with ref attribute) will be processed at second phase
				map<int, CommonAnim*> referencedAnims;

				int actualAnimIndex = 0;

				// phase 1: load not referenced animations
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
		* @param referencedAnims output array that will be filled with found referenced animations
		* @param actualAnimIndex index of actual animation being processed
		* @param builder instance that creates new instances of the animation object 
		*/
		CommonAnim* CreateAnimationFromXml(spt<ofxXml> xml, map<int, CommonAnim*>& referencedAnims, int& actualAnimIndex, SheetAnimBuilder builder) {

			int innerAnimations = xml->getNumTags("anim");

			CommonAnim* anim = builder();

			// fill ref and name attribute
			anim->LoadBaseAttributesFromXml(xml);

			if (anim->GetRef().length() != 0) {
				// animation is referenced -> push it to the referencedAnims collection and do nothing (it will be loaded during the second phase)
				referencedAnims[actualAnimIndex] = anim;
				if (innerAnimations != 0) throw ConfigErrorException("Referenced animations mustn't have inner animations!");

				return anim;
			}
			else {
				// children will be taken from referenced animation
				for (int i = 0; i < innerAnimations; i++) {
					xml->pushTag("anim", i);
					// process recursively
					spt<CommonAnim> newAnim = spt<CommonAnim>(CreateAnimationFromXml(xml, referencedAnims, ++actualAnimIndex, builder));
					anim->GetChildren().push_back(newAnim);
					xml->popTag();
				}
			}

			// set other attributes, using virtual method of the new CommonAnim object (each type of animation could have different attributes)
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
		* @rootAnimIndex index of actual root animation that is on the top of the tree
		* @rootAnims list of all root animations found during the first phase
		* @actualAnimIndex index of actual animation being processed
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
					// reference doesn't contain dot - it means that we can find its reference in actual scope (under the root animation)
					CommonAnim* reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference != nullptr) {
						// reference was found -> load other parameters
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference not found; but it still could be located under different root animation
						spt<CommonAnim> rootReference = FindAnimByName(ref, rootAnims);
						if (!rootReference) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));
						// reference was found -> load other parameters
						refAnim->GetParametersFromReference(rootReference);
					}
				}
				else {
					// ref attribute contains dot - it means that the animation will be in another scope
					string rootAnimName = ref.substr(0, ref.find("."));
					string subAnim = ref.substr(ref.find(".") + 1);
					// get sub-animation
					spt<CommonAnim> root = FindAnimByName(rootAnimName, rootAnims);
					CommonAnim* scopeAnim = root->FindChild(subAnim);
					if (!root || scopeAnim == nullptr) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

					// reference was found -> load other parameters
					refAnim->GetParametersFromReference(scopeAnim);
				}

				// fill remaining attributes from XML
				refAnim->LoadAttributesFromXml(xml);

			}
			else {
				// animation doesn't have ref attribute -> lets scan its children
				for (int i = 0; i < innerAnimations; i++) {
					xml->pushTag("anim", i);
					// recursion
					ProcessRefAnimationFromXml(xml, referencedAnims, rootAnimIndex, rootAnims, ++actualAnimIndex);
					xml->popTag();
				}
			}
		}

	};

} // namespace