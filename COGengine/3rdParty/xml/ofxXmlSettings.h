#pragma once

#include "ofMain.h"
#include <string.h>
#if (_MSC_VER)
#include "tinyxml.h"
#else
#include "tinyxml.h"
#endif

using namespace std;


#define MAX_TAG_VALUE_LENGTH_IN_CHARS		1024

class ofxXmlSettings: public ofBaseFileSerializer{

	public:
        ofxXmlSettings();
        ofxXmlSettings(const string& xmlFile);

        ~ofxXmlSettings();

		void setVerbose(bool _verbose);

		bool loadFile(const string& xmlFile);
		bool saveFile(const string& xmlFile);
		bool saveFile();

		bool load(const string & path);
		bool save(const string & path);

		void clearTagContents(const string& tag, int which = 0);
		void removeTag(const string& tag, int which = 0);

		bool tagExists(const string& tag, int which = 0);

		// removes all tags from within either the whole document
		// or the tag you are currently at using pushTag
		void	clear();

		bool	getBoolValue(const string& tag, bool			defaultValue, int which = 0);
		int 	getValue(const string&  tag, int            defaultValue, int which = 0);
		double 	getValue(const string&  tag, double         defaultValue, int which = 0);
		float 	getValue(const string&  tag, float         defaultValue, int which = 0);
		string 	getValue(const string&  tag, const string& 	defaultValue, int which = 0);

		bool	getBoolValuex(bool			defaultValue, int which = 0);
		int 	getValuex(int            defaultValue, int which = 0);
		double 	getValuex(double         defaultValue, int which = 0);
		float 	getValuex(float         defaultValue, int which = 0);
		string 	getValuex(const string& 	defaultValue, int which = 0);

		int		setValue(const string&	tag, bool			value, int which = 0);
		int 	setValue(const string&  tag, int            value, int which = 0);
		int 	setValue(const string&  tag, double         value, int which = 0);
		int 	setValue(const string&  tag, float         value, int which = 0);
		int 	setValue(const string&  tag, const string& 	value, int which = 0);

		//advanced

		//-- pushTag/popTag
		//pushing a tag moves you inside it which has the effect of
		//temporarily treating the tag you are in as the document root
		//all setValue, readValue and getValue commands are then be relative to the tag you pushed.
		//this can be used with addValue to create multiple tags of the same name within
		//the pushed tag - normally addValue only lets you create multiple tags of the same
		//at the top most level.

		bool	pushTag(const string&  tag, int which = 0);
		bool    pushTagIfExists(const string& tag, int which = 0);
		int		popTag();
		void    popAll();
		int		getPushLevel();

		//-- numTags
		//this only works for tags at the current root level
		//use pushTag and popTag to get number of tags whithin other tags
		// both getNumTags("PT"); and getNumTags("PT:X"); will just return the
		//number of <PT> tags at the current root level.
		int		getNumTags(const string& tag);

		//-- addValue/addTag
		//adds a tag to the document even if a tag with the same name
		//already exists - returns an index which can then be used to
		//modify the tag by passing it as the last argument to setValue

		//-- important - this only works for top level tags
		//   to put multiple tags inside other tags - use pushTag() and popTag()

		int 	addValue(const string&  tag, int            value);
		int 	addValue(const string&  tag, double         value);
		int 	addValue(const string&  tag, const string& 	value);

		int		addTag(const string& tag); //adds an empty tag at the current level

		void serialize(const ofAbstractParameter & parameter);
		void deserialize(ofAbstractParameter & parameter);


        // Attribute-related methods
		int		addAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		addAttribute(const string& tag, const string& attribute, int value);
		int		addAttribute(const string& tag, const string& attribute, double value);
		int		addAttribute(const string& tag, const string& attribute, const string& value);

		void	removeAttribute(const string& tag, const string& attribute, int which = 0);
		void	clearTagAttributes(const string& tag, int which = 0);

		int		getNumAttributes(const string& tag, int which = 0);

		bool	attributeExists(const string& tag, const string& attribute, int which = 0);
		bool    attributeExists(const string& attribute, int which = 0);

		bool    getAttributeNames(const string& tag, vector<string>& outNames, int which = 0);

		bool	getBoolAttribute(const string& tag, const string& attribute, bool defaultValue, int which = 0);
		int		getAttribute(const string& tag, const string& attribute, int defaultValue, int which = 0);
		double	getAttribute(const string& tag, const string& attribute, double defaultValue, int which = 0);
		string	getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which = 0);

		bool	getBoolAttributex(const string& attribute, bool defaultValue, int which = 0);
		int		getAttributex(const string& attribute, int defaultValue, int which = 0);
		double	getAttributex(const string& attribute, double defaultValue, int which = 0);
		string	getAttributex(const string& attribute, const string& defaultValue, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value);
		int		setAttribute(const string& tag, const string& attribute, double value);
		int		setAttribute(const string& tag, const string& attribute, const string& value);

		bool	loadFromBuffer( string buffer );
		void	copyXmlToString(string & str);

		TiXmlDocument 	doc;
		bool 			bDocLoaded;

	protected:

		TiXmlHandle     storedHandle;
		int             level;


		int 	writeTag(const string&  tag, const string& valueString, int which = 0);
		bool 	readTag(const string&  tag, TiXmlHandle& valHandle, int which = 0);	// max 1024 chars...


		int		writeAttribute(const string& tag, const string& attribute, const string& valueString, int which = 0);

        TiXmlElement* getElementForAttribute(const string& tag, int which);
        bool readIntAttribute(const string& tag, const string& attribute, int& valueString, int which);
        bool readDoubleAttribute(const string& tag, const string& attribute, double& outValue, int which);
        bool readStringAttribute(const string& tag, const string& attribute, string& outValue, int which);
};   

typedef ofxXmlSettings ofxXml;