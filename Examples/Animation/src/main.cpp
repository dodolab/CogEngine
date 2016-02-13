#include "ofxCogMain.h"


class XmlTestingApp : public CogApp {


	void InitComponents() {

	}

	void InitEngine() {
		ofxXml* xml = new ofxXml();
		xml->loadFile("config.xml");
		auto xmlPtr = spt<ofxXml>(xml);
		
		CogEngine::GetInstance().Init(xmlPtr);
	}
};


 
int main() {

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new XmlTestingApp());

	return 0;
}


