#pragma once
#include "CogNode.h"
#include "CogSceneManager.h"

namespace Cog {

	/*! Calculation type */
	enum class CogCalcType {
		ABS,	/*!< absolute */
		LOC,	/*!< local */
		PER 	/*!< percentage <0..1> */
	};

	/**
	* Factory with methods for calculating positions
	* All node construction methods should be implemented
	* in classes derived from this one
	*/
	class CogFactory {

	public:

		// scene manager, assigned to the root object
		CogSceneManager* sceneManager;

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
		CogTrans CalcTransform(CogNode* node, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, int width, int height, CogNode* parent);

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
		void SetTransform(CogNode* node, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, int width, int height, CogNode* parent);

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
		spt<ofImage> SetRenderImage(CogNode* node, string imgPath, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent);

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
		CogNode* CreateImageNode(string tag, string imgPath, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent);

		/**
		* Sets font to node
		* @param node node to set
		* @param font font to set
		* @param color font color
		* @param text text to set
		*/
		void SetFont(CogNode* node, spt<ofTrueTypeFont> font, ofColor color, string text);

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
		void SetRenderFont(CogNode* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent);

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
		CogNode* CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent);

		/**
		* Calculates position
		* @param node node to calculate
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param parent link to the object parent
		*/
		ofVec2f CalcPosition(CogNode* node, ofVec2f pos, CogCalcType posCalc, CogNode* parent);

		/**
		* Calculates position
		* @param node node to calculate
		* @param pos position, calculated due to posCalc parameter
		* @param posCalc position calculation type (absolute, percentage or local)
		* @param anchor percentage object anchor
		* @param parent link to the object parent
		*/
		ofVec2f CalcPosition(CogNode* node, ofVec2f pos, CogCalcType posCalc, ofVec2f anchor, int width, int height, CogNode* parent);

		/**
		* Calculates scale
		* @param node node to calculate
		* @param scaleX scale in x axis
		* @param width object width
		* @param scaleCalc scale calculation type (absolute, percentage or local)
		* @param parent link to the object parent
		*/
		ofVec3f CalcScale(CogNode* node, float scaleX, float width, CogCalcType scaleCalc, CogNode* parent);

		/**
		* Gets translating speed according to the actual screen width
		* @return speed in X pixels per second, where X is screen width
		*/
		float TranslateSpeed(float speed);

		/**
		* Initializes factory
		*/
		virtual void Init() {

		}

		/**
		* Creates root node with initial scene
		*/
		virtual CogNode* CreateRoot();


		/**
		* Loads all animations into storage
		* @param xml entity with animations
		* @return true, if no error has occured
		*/
		bool LoadAnimations(spt<ofxXmlSettings> xml);

	};

}