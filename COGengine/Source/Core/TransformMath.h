#pragma once

#include "TransformEnt.h"
#include "Transform.h"

namespace Cog {

	class Node;
	class Settings;

	/**
	* Transformation helper, calculates transform based on configuration in TransformEnt entity
	* All transformations in the engine are kept as either local or absolute; therefore an additional
	* calculations must take place when you want to calculate for instance position in the center of a parent
	* object with relative anchor attached to the topleft corner of the child
	*/
	class TransformMath  {

	public:

		/**
		* Sets size to fit the screen
		*/
		void SetSizeToScreen(Node* node, Node* parent);

		/**
		* Sets transform to given node, with the accordance of configured TransformEnt entity
		* @param node node to configure
		* @param parent node's parent
		* @param entity configuration entity
		* @param gridWidth width of the scene grid (if specified)
		* @param gridHeight height of the scene grid (if specified)
		*/
		void SetTransform(Node* node, Node* parent, TransformEnt entity, int gridWidth = 0, int gridHeight = 0);

		/**
		* Calculates transform of given node, with the accordance of configured TransformEnt entity
		* @param outputTrans output transformation
		* @param node node to configure
	    * @param parent node's parent
		* @param entity configuration entity
		* @param gridWidth width of the scene grid (if specified)
		* @param gridHeight height of the scene grid (if specified)
		*/
		void CalcTransform(Trans& outputTrans, Node* node, Node* parent, TransformEnt entity, int gridWidth = 0, int gridHeight = 0);

		/**
		* Calculates position of given node, according to the parameters
		* @param node node to calculate
		* @param parent node's parent
		* @param pos position
		* @param posCalc type of position calculation (relative, absolute etc., see CalcType enum)
		* @param gridWidth width of the scene grid (if specified)
		* @param gridHeight height of the scene grid (if specified)
		*/
		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth = 0, int gridHeight = 0);

		/**
		* Calculates scale of given node, according to the parameters
		* @param node node to calculate
		* @param parent node's parent
		* @param width desired width
		* @param height desired height
		* @param scaleCalc type of scale calculation (relative, absolute etc., see CalcType enum)
		* @param gridWidth width of the scene grid (if specified)
		* @param gridHeight height of the scene grid (if specified)
		*/
		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth = 0, int gridHeight = 0);
		
		/**
		* Gets center of the screen
		*/
		ofVec2f GetCenter();
	};

}// namespace