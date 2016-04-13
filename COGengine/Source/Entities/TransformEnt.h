#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"

namespace Cog {

	/**
	* Type of the transformation calculation
	*/
	enum class CalcType {
		ABS,	/** absolute (un) */
		LOC,	/** local (-) */
		PER,	/** percentage <0..1> (r) */
		GRID,	/** grid positioning (gr) */
		ABS_PER /** absolute percentage (rp) */
	};

	/**
	* Transformation entity that holds attributes required for transformation calculation such as position vectors, size vectors,
	* rotation, calculation types (local, absolute, percentage) etc.
	*/
	class TransformEnt : public DEntity{
	public:

		ofVec2f pos;
		int zIndex;
		CalcType pType;
		ofVec2f anchor;
		ofVec2f size;
		ofVec2f rotationCentroid;
		CalcType sType;
		float rotation;

		TransformEnt() : pos(0), zIndex(0), pType(CalcType::PER), anchor(0), 
			size(1), sType(CalcType::LOC), rotation(0), rotationCentroid(0.5f){
		}

		TransformEnt(string name) : TransformEnt() {
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param name entity name
		* @param position local position
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param rotation local rotation in degrees
		*/
		TransformEnt(string name, ofVec2f position, CalcType positionCalc, float rotation) : TransformEnt(){
			this->name = name;
			this->pos = position;
			this->pType = positionCalc;
			this->rotation = rotation;
		}

		/**
		* Creates a new transformation entity
		* @param position local position
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param rotation local rotation in degrees
		*/
		TransformEnt(ofVec2f position, CalcType positionCalc, float rotation) : TransformEnt() {
			this->pos = position;
			this->pType = positionCalc;
			this->rotation = rotation;
		}

		/**
		* Creates a new transformation entity
		* @param name entity name
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		*/
		TransformEnt(string name, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc) :pos(position), 
			zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(0), rotationCentroid(0.5f){
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		*/
		TransformEnt(ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc) :pos(position),
			zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(0), rotationCentroid(0.5f) {
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param name entity name
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		* @param rotation local rotation in degrees
		*/
		TransformEnt(string name, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation):pos(position),
			zIndex(zIndex),pType(positionCalc),anchor(anchor),
			size(size),sType(sizeCalc), rotation(rotation), rotationCentroid(0.5f){
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		* @param rotation local rotation in degrees
		*/
		TransformEnt(ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation) :pos(position),
			zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(rotation), rotationCentroid(0.5f) {
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param name entity name
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		* @param rotation local rotation in degrees
		* @param rotationCentroid local center of rotation 
		*/
		TransformEnt(string name, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation, ofVec2f rotationCentroid)
			:pos(position), zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(rotation), rotationCentroid(rotationCentroid) {
			this->name = name;
		}

		/**
		* Creates a new transformation entity
		* @param position local position
		* @param zIndex local z-index
		* @param positionCalc type of position calculation (local, absolute, percentage,...)
		* @param anchor percentage anchor of the center of the calculation (0.5f for center, 0 for topleft edge)
		* @param size local size
		* @param sizeCalc type of size calculation (local, absolute, percentage, ...)
		* @param rotation local rotation in degrees
		* @param rotationCentroid local center of rotation
		*/
		TransformEnt(ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation, ofVec2f rotationCentroid)
			:pos(position), zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(rotation), rotationCentroid(rotationCentroid) {
			this->name = name;
		}

		TransformEnt(const TransformEnt& copy) {
			this->name = copy.name;
			this->anchor = copy.anchor;
			this->pos = copy.pos;
			this->pType = copy.pType;
			this->rotation = copy.rotation;
			this->rotationCentroid = copy.rotationCentroid;
			this->size = copy.size;
			this->sType = copy.sType;
			this->zIndex = copy.zIndex;
		}

		~TransformEnt() {

		}

		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings);

	};


}// namespace


