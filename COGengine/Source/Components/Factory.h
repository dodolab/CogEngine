#pragma once
#include "Node.h"
#include "SceneManager.h"

namespace Cog {

	/*! Calculation type */
	enum class CalcType {
		ABS,	/*!< absolute */
		LOC,	/*!< local */
		PER, 	/*!< percentage <0..1> */
		GRID,    /*!< grid positioning, grid size must be specified */
		ABS_PER /*!< absolute percentage */
	};

	/**
	* Factory with methods for calculating positions
	* All node construction methods should be implemented
	* in classes derived from this one
	*/
	class Factory : public Component {

		OBJECT(Factory)

	public:

		// scene manager, assigned to the root object
		SceneManager* sceneManager;

		/**
		* Gets position of screen center
		*/
		ofVec2f GetCenter();

		/**
		* Calculates transform of selected node
		* @param node node to calculate
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param width object width
		* @param height object height
		* @param parent link to the object parent
		*/
		Trans CalcTransform(Node* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, Node* parent);

		/**
		* Sets absolute transform to node
		* @param node node to set
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param width object width
		* @param height object height
		* @param parent link to the object parent
		*/
		void SetTransform(Node* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, Node* parent);

		/**
		* Loads render image and loads it to the node with calculated transformation
		* @param node node to set
		* @param imgPath path to the image
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		spt<ofImage> SetRenderImage(Node* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent);

		/**
		* Creates an image node and sets it as a child of selected parent
		* @param tag object tag
		* @param imgPath path to the image
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		Node* CreateImageNode(string tag, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent);

		/**
		* Sets font to node
		* @param node node to set
		* @param font font to set
		* @param color font color
		* @param text text to set
		*/
		void SetFont(Node* node, spt<ofTrueTypeFont> font, ofColor color, string text);

		/**
		* Loads render font and loads it to the node with calculated transformation
		* @param node node to set
		* @param font font to set
		* @param color font color
		* @param text text to set
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		void SetRenderFont(Node* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent);

		/**
		* Creates an font node and sets it as a child of selected parent
		* @param tag object tag
		* @param font font to set
		* @param color font color
		* @param text text to set
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param scaleX scale in x axis
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		Node* CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent);

		/**
		* Calculates position
		* @param node node to calculate
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param parent link to the object parent
		*/
		ofVec2f CalcPosition(Node* node, ofVec2f pos, CalcType posCalc, Node* parent);

		/**
		* Calculates position
		* @param node node to calculate
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		ofVec2f CalcPosition(Node* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, int width, int height, Node* parent);

		/**
		* Calculates scale
		* @param node node to calculate
		* @param scaleX scale in x axis
		* @param width object width
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param parent link to the object parent
		*/
		ofVec3f CalcScale(Node* node, float scaleX, float width, CalcType scaleCalc, Node* parent);

		/**
		* Gets translating speed according to the actual screen width
		* @return speed in X pixels per second, where X is screen width
		*/
		float TranslateSpeed(float speed);


		/**
		* Creates root node with initial scene
		*/
		virtual Node* CreateRoot();


		/**
		* Loads all animations into storage
		* @param xml entity with animations
		* @return true, if no error has occured
		*/
		bool LoadAnimations(spt<ofxXmlSettings> xml);

	};

}// namespace