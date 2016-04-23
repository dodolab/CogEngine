#pragma once

#include "Definitions.h"
#include "AttrAnimEnt.h"
#include "ofxXmlSettings.h"
#include "GeneralAnim.h"

namespace Cog {

	/**
	* Container for context of an animation entity
	* Contains from-to values that are calculated according to the 
	* actual state of the transformation node
	*/
	class AttribAnimContext {
	private:
		spt<AttrAnimEnt> entity;
		// starting value
		float fromVal;
		// ending value
		float toVal;
		bool initialized = false;

	public:

		AttribAnimContext(spt<AttrAnimEnt> entity) : entity(entity) {

		}

		/**
		* Initializes the context
		*/
		void Init(float fromVal, float toVal);

		/**
		* Returns true, if the context is initialized
		*/
		bool IsInitialized() const {
			return initialized;
		}

		/**
		* Gets the animation entity
		*/
		spt<AttrAnimEnt> GetEntity() {
			return entity;
		}

		/**
		* Gets the starting animation value
		*/
		float GetFromValue() const {
			return fromVal;
		}

		/**
		* Gets the ending animation value
		*/
		float GetToValue() const {
			return toVal;
		}
	};

	/**
	* Entity for attribute animations
	*/
	class AttribAnim : public GeneralAnim {
	protected:
		vector<AttribAnimContext> animEntities;
		int duration = 0;
	public:

		AttribAnim() {

		}

		/**
		* Creates a new attribute animation
		* @param name animation name
		* @param ref name of referenced animation
		* @param speed animation speed (in multiple units)
		* @param repeat number of repetitions (0 for infinite)
		* @param isRevert indicator whether the animation is reverted
		*/
		AttribAnim(string name, string ref, float speed, int repeat, bool isRevert);

		/**
		* Creates a new attribute animation
		* @param name animation name
		* @param ref name of referenced animation
		* @param speed animation speed (in multiple units)
		* @param repeat number of repetitions (0 for infinite)
		* @param isRevert indicator whether the animation is reverted
		* @param animEntities collection of entities with animated attributes
		*/
		AttribAnim(string name, string ref, float speed, int repeat, bool isRevert, vector<AttribAnimContext>& animEntities);

		virtual ~AttribAnim() {
		}

		/**
		* Recalculates duration as the maximum duration of all children
		*/
		void RecalcDuration();

		/**
		* Copies all parameters from referenced animation
		*/
		void GetParametersFromReference(spt<GeneralAnim> reference);

		/**
		* Loads attributes from xml
		*/
		virtual void LoadAttributesFromXml(spt<ofxXml> xml);

		virtual bool IsAnimatable() {
			return HasAnimEntities();
		}

		virtual bool IsContinous() {
			return true;
		}

		virtual int GetDuration() {
			return duration;
		}

		void SetDuration(int duration) {
			this->duration = duration;
		}

		/**
		* Returns true, if the animation has entities to animate
		*/
		bool HasAnimEntities() {
			return this->animEntities.size() != 0;
		}

		/**
		* Gets collection of animation entities
		*/
		vector<AttribAnimContext>& GetAnimEntities() {
			return this->animEntities;
		}
	};



}// namespace