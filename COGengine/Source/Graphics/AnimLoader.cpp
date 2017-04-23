#include "AnimLoader.h"
#include "Error.h"
#include "Utils.h"

namespace Cog {

		void AnimLoader::LoadAnimationsFromXml(xml_node& xml, vector<spt<GeneralAnim>>& rootAnims, SheetAnimBuilder builder) {

			// referenced anims (those with ref attribute) will be processed in second phase
			map<int, GeneralAnim*> referencedAnims;

			int actualAnimIndex = 0;

			// phase 1: load direct animations (not referenced)
			for (auto animNode : xml.children("anim")) {
				// load
				spt<GeneralAnim> anim = spt<GeneralAnim>(CreateAnimationFromXml(animNode, referencedAnims, actualAnimIndex, builder));
				if (anim->GetName().length() == 0) throw ConfigErrorException("All root animations must have a name!");

				rootAnims.push_back(anim);
			}

			actualAnimIndex = 0;
			// phase 2: load referenced animations
			int indexCounter = 0;
			for (auto animNode : xml.children("anim")) {
				ProcessRefAnimationFromXml(animNode, referencedAnims, indexCounter++, rootAnims, actualAnimIndex);
			}
		}

		GeneralAnim* AnimLoader::CreateAnimationFromXml(xml_node& xml, map<int, GeneralAnim*>& referencedAnims, int& actualAnimIndex, SheetAnimBuilder builder) {

			auto innerAnimations = xml.children("anim");
			
			GeneralAnim* anim = builder();

			// fill ref and name attribute
			anim->LoadBaseAttributesFromXml(xml);

			if (anim->GetRef().length() != 0) {
				// animation is referenced -> push it to the referencedAnims collection and do nothing (it will be loaded during the second phase)
				referencedAnims[actualAnimIndex] = anim;
				if (innerAnimations.begin() != innerAnimations.end()) throw ConfigErrorException("Referenced animations mustn't have inner animations!");

				return anim;
			}
			else {
				// children will be taken from referenced animation
				for (auto innerAnim : innerAnimations) {
					// process recursively
					spt<GeneralAnim> newAnim = spt<GeneralAnim>(CreateAnimationFromXml(innerAnim, referencedAnims, ++actualAnimIndex, builder));
					anim->GetChildren().push_back(newAnim);
				}
			}

			// set other attributes, using virtual method of the new GeneralAnim object (each type of animation could have different attributes)
			anim->LoadAttributesFromXml(xml);

			return anim;
		}

		spt<GeneralAnim> AnimLoader::FindAnimByName(string name, vector<spt<GeneralAnim>>& anims) {
			for (auto it = anims.begin(); it != anims.end(); ++it) {
				spt<GeneralAnim> anim = (*it);
				if (anim->GetName().compare(name) == 0) return anim;
			}

			return spt<GeneralAnim>();
		}

		void AnimLoader::ProcessRefAnimationFromXml(xml_node& xml, map<int, GeneralAnim*>& referencedAnims,
			int rootAnimIndex, vector<spt<GeneralAnim>>& rootAnims, int& actualAnimIndex) {

		
			string ref = xml.attribute("ref").as_string();
			string name = xml.attribute("name").as_string();

			if (ref.length() != 0) {
				// got referenced animation
				auto refAnim = referencedAnims[actualAnimIndex];

				if (ref.find(".") == -1) {
					// reference doesn't contain dot - it means that we can find its reference in actual scope (under the root animation)
					auto reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference) {
						// reference was found -> load other parameters
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference not found; but it still could be located under different root animation
						spt<GeneralAnim> rootReference = FindAnimByName(ref, rootAnims);
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
					spt<GeneralAnim> root = FindAnimByName(rootAnimName, rootAnims);
					auto scopeAnim = root->FindChild(subAnim);
					if (!root || !scopeAnim) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

					// reference was found -> load other parameters
					refAnim->GetParametersFromReference(scopeAnim);
				}

				// fill remaining attributes from XML
				refAnim->LoadAttributesFromXml(xml);

			}
			else {
				// animation doesn't have ref attribute -> lets scan its children
				for (auto innerAnim : xml.children("anim")) {
					// recursion
					ProcessRefAnimationFromXml(innerAnim, referencedAnims, rootAnimIndex, rootAnims, ++actualAnimIndex);
				}
			}
		}


} // namespace