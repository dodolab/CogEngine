#include "ofxCogMain.h"


class XmlTestingApp : public CogApp {


	void InitComponents() {

	}

	void InitEngine() {
		ofxXml* xml = new ofxXml();
		xml->loadFile("config.xml");
		auto xmlPtr = spt<ofxXml>(xml);
		
		COGEngine.Init(xmlPtr);


		xmlPtr->popAll();
		xmlPtr->pushTag("app_config");
		xmlPtr->pushTag("scenes");


		auto context = GETCOMPONENT(SceneContext);
		context->LoadScenesFromXml(xmlPtr);
		xmlPtr->popTag();
	}
};


 
int main() {

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new XmlTestingApp());

	return 0;
}


