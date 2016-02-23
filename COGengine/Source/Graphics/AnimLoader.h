#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"

namespace Cog {

	typedef std::function<CommonAnim*()> SheetAnimBuilder;


	/**
	* Class that loads animations from XML
	*/
	class AnimLoader {

	public:

		/**
		* Loads animation from xml
		* @param xml xml to load from
		* @param rootAnims output collection that will contain all animations on the top of the tree
		* @param builder instance that creates new instances of the animation object
		*/
		void LoadAnimationsFromXml(spt<ofxXml> xml, vector<spt<CommonAnim>>& rootAnims, SheetAnimBuilder builder);

	private:

		/**
		* Creates animation from XML
		* @param xml xml to load from
		* @param referencedAnims output array that will be filled with found referenced animations
		* @param actualAnimIndex index of actual animation being processed
		* @param builder instance that creates new instances of the animation object
		*/
		CommonAnim* CreateAnimationFromXml(spt<ofxXml> xml, map<int, CommonAnim*>& referencedAnims, int& actualAnimIndex, SheetAnimBuilder builder);

		/**
		* Finds animation by name, in selected collection
		*/
		spt<CommonAnim> FindAnimByName(string name, vector<spt<CommonAnim>> anims);

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of actual root animation that is on the top of the tree
		* @rootAnims list of all root animations found during the first phase
		* @actualAnimIndex index of actual animation being processed
		*/
		void ProcessRefAnimationFromXml(spt<ofxXml> xml, map<int, CommonAnim*>& referencedAnims,
			int rootAnimIndex, vector<spt<CommonAnim>>& rootAnims, int& actualAnimIndex);

	};

} // namespace