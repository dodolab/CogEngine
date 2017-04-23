#pragma once

#include "DEntity.h"
#include "Settings.h"

namespace Cog {

	/**
	* Transformation entity that holds attributes required for transformation calculation such as position vectors, size vectors,
	* rotation, calculation types (local, absolute, percentage) etc.
	*/
	class TransformEnt : public DEntity{
	public:

		ofVec2f pos;
		int zIndex;
		stenum pType;
		ofVec2f anchor;
		ofVec2f size;
		ofVec2f rotationCentroid;
		stenum sType;
		float rotation;

		TransformEnt() : pos(0), zIndex(0), pType(CALCTYPE_PER), anchor(0), 
			size(1), sType(CALCTYPE_LOC), rotation(0), rotationCentroid(0.5f){
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
		TransformEnt(string name, ofVec2f position, stenum positionCalc, float rotation) : TransformEnt(){
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
		TransformEnt(ofVec2f position, int positionCalc, float rotation) : TransformEnt() {
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
		TransformEnt(string name, ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc) :pos(position),
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
		TransformEnt(ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc) :pos(position),
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
		TransformEnt(string name, ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc, float rotation):pos(position),
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
		TransformEnt(ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc, float rotation) :pos(position),
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
		TransformEnt(string name, ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc, float rotation, ofVec2f rotationCentroid)
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
		TransformEnt(ofVec2f position, int zIndex, stenum positionCalc,
			ofVec2f anchor, ofVec2f size, stenum sizeCalc, float rotation, ofVec2f rotationCentroid)
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

		TransformEnt& SetPosition(ofVec2f pos, stenum pType) {
			this->pos = pos;
			this->pType = pType;
			return *this;
		}

		TransformEnt& SetLocalPosition(ofVec2f pos) {
			return SetPosition(pos, CALCTYPE_LOC);
		}

		TransformEnt& SetRelPosition(ofVec2f pos) {
			return SetPosition(pos, CALCTYPE_PER);
		}

		TransformEnt& SetAbsPosition(ofVec2f pos) {
			return SetPosition(pos, CALCTYPE_ABS);
		}

		TransformEnt& SetGridPosition(ofVec2f pos) {
			return SetPosition(pos, CALCTYPE_GRID);
		}


		TransformEnt& SetZIndex(int zIndex) {
			this->zIndex = zIndex;
			return *this;
		}

		TransformEnt& SetAnchor(ofVec2f anchor) {
			this->anchor = anchor;
			return *this;
		}

		TransformEnt& SetAnchorCenter() {
			this->anchor = ofVec2f(0.5f);
			return *this;
		}

		TransformEnt& SetAnchorBottomRight() {
			this->anchor = ofVec2f(1);
			return *this;
		}

		TransformEnt& SetScale(ofVec2f scale, stenum sType) {
			this->size = scale;
			this->sType = sType;
			return *this;
		}

		TransformEnt& SetLocalScale(ofVec2f scale) {
			return SetScale(scale, CALCTYPE_LOC);
		}

		TransformEnt& SetRelScale(ofVec2f scale) {
			return SetScale(scale, CALCTYPE_PER);
		}

		TransformEnt& SetAbsScale(ofVec2f scale) {
			return SetScale(scale, CALCTYPE_ABS);
		}

		TransformEnt& SetGridScale(ofVec2f scale) {
			return SetScale(scale, CALCTYPE_GRID);
		}

		TransformEnt& SetRotation(float rotation) {
			this->rotation = rotation;
			return *this;
		}

		TransformEnt& SetRotCentroid(ofVec2f centroid) {
			this->rotationCentroid = centroid;
			return *this;
		}

		void LoadFromXml(xml_node& xml, Setting& defaultSettings);

	};


}// namespace


