#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	/**
	* Entity for common animations
	*/
	class CommonAnim {
	protected:
		// name of animation
		string name = "";
		// reference to another animation
		string ref = "";
		// speed multiplier
		float speed = 1;
		// number of repetitions
		int repeat = 1;
		// if true, animation will be reverted
		bool isRevert = false;
		// list of children
		vector<spt<CommonAnim>> children;

	public:

		CommonAnim() {

		}

		CommonAnim(string name, string ref, float speed, int repeat, bool isRevert) : name(name), ref(ref),
			speed(speed), repeat(repeat), isRevert(isRevert) {

		}

		virtual ~CommonAnim() {
		}


		/**
		* Copies all parameters from other animation
		*/
		virtual void GetParametersFromReference(CommonAnim* reference) {
			this->SetSpeed(reference->GetSpeed());
			this->SetRepeat(reference->GetRepeat());
			this->SetIsRevert(reference->GetIsRevert());

			// insert children
			vector<spt<CommonAnim>>& refChildren = reference->children;

			for (auto it = refChildren.begin(); it != refChildren.end(); ++it) {
				spt<CommonAnim> child = (*it);
				AddChild(child);
			}
		}


		/**
		* Returns all nodes in the tree (including this node)
		*/
		void GetAllNodes(vector<CommonAnim*> &output) {
			output.push_back(this);

			for (auto it = children.begin(); it != children.end(); ++it) {
				spt<CommonAnim> an = (*it);
				an->GetAllNodes(output);
			}
		}

		/**
		* Finds a child by name recursively
		* @param name according to find the child
		*/
		CommonAnim* FindChild(string name) {
			if (this->GetName().compare(name) == 0) return this;

			for (auto it = children.begin(); it != children.end(); ++it) {
				if ((*it)->GetName().compare(name) == 0) return (*it);
				else {
					auto childFound = (*it)->FindChild(name);
					if (childFound != nullptr) return childFound;
				}
			}

			return nullptr;
		}

	
		/**
		* Adds a new animation child
		* @param child child to add
		* @return true, if child was added (wasn't included in list of children yet)
		*/
		bool AddChild(spt<CommonAnim> child) {
			auto found = find(children.begin(), children.end(), child);
			if (found != children.end()) {
				return false;
			}
			else {
				children.push_back(child);
				return true;
			}
		}

		/**
		* Removes animation child
		* @param child child to remove
		* @return true, if child has been removed (was included in list of children)
		*/
		bool RemoveChild(spt<CommonAnim> child) {
			auto found = find(children.begin(), children.end(), child);
			if (found != children.end()) {
				children.erase(found);
				return true;
			}
			else return false;
		}

		/**
		* Loads base attributes such as ref and name from XML
		*/
		virtual void LoadBaseAttributesFromXml(spt<ofxXml> xml) {
			this->SetRef(xml->getAttribute(":", "ref", this->GetRef()));
			this->SetName(xml->getAttribute(":", "name", this->GetName()));
		}

		/**
		* Loads other attributes from XML
		*/
		virtual void LoadAttributesFromXml(spt<ofxXml> xml) {
			this->SetSpeed(xml->getAttribute(":", "speed", this->GetSpeed()));
			if (this->GetSpeed() < 0) throw IllegalArgumentException(string_format("Error in animation %s; speed bust be greater than 0", this->GetName().c_str()));
			this->SetRepeat(xml->getAttribute(":", "repeat", this->GetRepeat()));
			if (this->GetRepeat() < 0) throw IllegalArgumentException(string_format("Error in animation %s; number of repetitions must be greater or equal to 0", this->GetName().c_str()));
			this->SetIsRevert(xml->getBoolAttribute(":", "revert", this->GetIsRevert()));
		}

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
		const string GetName() const {
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
		const string GetRef() const {
			return ref;
		}

		/**
		* Sets name of referenced animation
		*/
		void SetRef(string ref) {
			this->ref = ref;
		}

		/**
		* Gets speed
		*/
		const float GetSpeed() const {
			return speed;
		}

		/**
		* Sets speed
		*/
		void SetSpeed(float speed) {
			this->speed = speed;
		}

		/**
		* Gets number of repetitions
		*/
		const int GetRepeat() const {
			return repeat;
		}

		/**
		* Sets number of repetitions
		*/
		void SetRepeat(int repeat) {
			this->repeat = repeat;
		}

		/**
		* Gets indicator, if animation is inverted
		*/
		const bool GetIsRevert() const {
			return isRevert;
		}

		/**
		* Sets indicator, if animation is inverted
		*/
		void SetIsRevert(bool isRevert) {
			this->isRevert = isRevert;
		}

		/**
		* Gets collection of children
		*/
		vector<spt<CommonAnim>>& GetChildren() {
			return children;
		}

	};



}// namespace