#pragma once

#include <string>
#include <vector>
using namespace std;

#include "Definitions.h"
#include "ofxXmlSettings.h"


namespace Cog {

	/**
	* General entity for animations
	*/
	class GeneralAnim {
	protected:
		// name of animation
		string name = "";
		// name of referenced animation
		string ref = "";
		// speed multiplier
		float speed = 1;
		// number of repetitions
		int repeat = 1;
		// if true, animation will be inverted
		bool isRevert = false;
		// collection of children
		vector<spt<GeneralAnim>> children;

	public:

		GeneralAnim() {

		}

		GeneralAnim(string name, string ref, float speed, int repeat, bool isRevert) 
			: name(name), ref(ref), speed(speed), repeat(repeat), isRevert(isRevert) {

		}

		virtual ~GeneralAnim() {
		}


		/**
		* Copies all parameters from other animation
		*/
		virtual void GetParametersFromReference(spt<GeneralAnim> reference);


		/**
		* Returns all nodes in the tree (including this node)
		* @param output output collection
		*/
		void GetAllNodes(vector<GeneralAnim*> &output);
		
		/**
		* Finds a child by name recursively
		* @param name child name
		*/
		spt<GeneralAnim> FindChild(string name);

	
		/**
		* Adds a new animation child
		* @param child child to add
		* @return true, if child was added (wasn't included in list of children yet)
		*/
		bool AddChild(spt<GeneralAnim> child);

		/**
		* Removes animation child
		* @param child child to remove
		* @return true, if child has been removed (was included in list of children)
		*/
		bool RemoveChild(spt<GeneralAnim> child);

		/**
		* Loads base attributes such as ref and name from XML
		*/
		virtual void LoadBaseAttributesFromXml(spt<ofxXml> xml);

		/**
		* Loads other attributes from XML
		*/
		virtual void LoadAttributesFromXml(spt<ofxXml> xml);

		/**
		* Returns true, if this animation is animatable (has animation content)
		*/
		virtual bool IsAnimatable() = 0;

		/**
		* Returns true, if this animations is continuous (used in continuous animations such as transformations)
		*/
		virtual bool IsContinous() = 0;

		/**
		* Gets duration (gets number of frames for discrete animation)
		*/
		virtual int GetDuration() = 0;

		/**
		* Gets name of this animation
		*/
		string GetName() const {
			return name;
		}

		/**
		* Sets name of this animation
		*/
		void SetName(string name) {
			this->name = name;
		}

		/**
		* Gets name of referenced animation
		*/
		string GetRef() const {
			return ref;
		}

		/**
		* Sets name of referenced animation
		*/
		void SetRef(string ref) {
			this->ref = ref;
		}

		/**
		* Gets speed multiplier
		*/
		float GetSpeed() const {
			return speed;
		}

		/**
		* Sets speed multiplier
		*/
		void SetSpeed(float speed) {
			this->speed = speed;
		}

		/**
		* Gets number of repetitions
		*/
		int GetRepeat() const {
			return repeat;
		}

		/**
		* Sets number of repetitions
		*/
		void SetRepeat(int repeat) {
			this->repeat = repeat;
		}

		/**
		* Gets indicator whether the animation is inverted
		*/
		bool GetIsRevert() const {
			return isRevert;
		}

		/**
		* Sets indicator whether the animation is inverted
		*/
		void SetIsRevert(bool isRevert) {
			this->isRevert = isRevert;
		}

		/**
		* Gets collection of children
		*/
		vector<spt<GeneralAnim>>& GetChildren() {
			return children;
		}

	};



}// namespace