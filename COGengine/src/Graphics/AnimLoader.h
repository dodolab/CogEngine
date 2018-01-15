#pragma once

#include <vector>
#include "Definitions.h"
#include "GeneralAnim.h"
#include "pugixml.h"
#include <functional>

using namespace pugi;

namespace Cog {

	typedef std::function<GeneralAnim*()> SheetAnimBuilder;

	/**
	* Class that loads animations from XML
	*/
	class AnimLoader {

	public:

		/**
		* Loads animation from xml
		* @param xml xml to load from
		* @param rootAnims output collection that will contain all animations on the top of the tree
		* @param builder builder that creates new instances of the animation object
		*/
		void LoadAnimationsFromXml(xml_node& xml, vector<spt<GeneralAnim>>& rootAnims, SheetAnimBuilder builder);

	private:

		/**
		* Creates animation from XML
		* @param xml xml to load from
		* @param referencedAnims output array that will be filled with found referenced animations
		* @param actualAnimIndex index of actual animation being processed
		* @param builder builder that creates new instances of the animation object
		*/
		GeneralAnim* CreateAnimationFromXml(xml_node& xml, map<int, GeneralAnim*>& referencedAnims,
			int& actualAnimIndex, SheetAnimBuilder builder);

		/**
		* Finds animation by name in given collection
		*/
		spt<GeneralAnim> FindAnimByName(string name, vector<spt<GeneralAnim>>& anims);

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of actual root animation that is on the top of the tree
		* @rootAnims list of all root animations found during the first phase
		* @actualAnimIndex index of actual animation being processed
		*/
		void ProcessRefAnimationFromXml(xml_node& xml, map<int, GeneralAnim*>& referencedAnims,
			int rootAnimIndex, vector<spt<GeneralAnim>>& rootAnims, int& actualAnimIndex);

	};

} // namespace