#pragma once


namespace Cog {

#define REGISTER_XMLHANDLING(element) \
  string XmlHandlerName() {\
    return string(element);\
  }

#define COMPONENT(compName) \
  public: \
  string GetClassName() { \
      return GetClassNameStatic(); \
  } \
  \
  static string GetClassNameStatic() { \
	  return string(#compName); \
  }

	/**
	* Common class for all components
	*/
	class Component {
	private:
		
	public:

		virtual string GetClassName() {
			return string();
		}

		/**
		* Initializes factory
		*/
		virtual void Init() {

		}

		/**
		* Initializes factory, using xml configuration
		*/
		virtual void Init(spt<ofxXmlSettings> config) {

		}

		/**
		* Gets name of xml handler
		*/
		virtual string XmlHandlerName() {
			return string();
		}
	};

}// namespace