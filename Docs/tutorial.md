CogEngine tutorial
===================

## Installing CogEngine
The simplest way is to install CogEngine as an Openframeworks addon. All includes will be generated automatically via the ProjectGenerator application.

Preferred way for Windows is to edit all *.sln files and set path to OpenFrameworks library (you can get OF here: http://openframeworks.cc/). Then you may open it in Visual Studio 2017 and compile it, wherever it is located. In any case the MS Visual C++ Redistributable 2015 and 2017 package is required to compile all projects, including OpenFrameworks.

Some projects may contain a data. In such case the data folder has to be copied into the bin directory after the compilation (Visual Studio has a script for it).

## Your first project
CogEnginelab provides everything you need to run an empty project using CogEngine and compile it for both Android and Windows. 

For Android, don't forget to set all paths in all property files (project.properties, local.properties), including path to ofxAndroid library and Android SDK.

Following code creates a simple application with empty scene:

```c++

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().Init("config.xml");
		ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));
	}

	void InitStage(Stage* stage) {
	}
};

#ifndef WIN32
#include <jni.h>
#endif

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}

#ifndef WIN32

//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif



```

## Executing tests

CogEngine has more than 200 automated tests you may check out in order to get under the hood. Tests are part of CogEngine itself but they have to be executed using the Catch framework. Project that executes all tests is located in the Tests folder.

## Examples

"Examples" folder contains some example project that shows some possibilities of the CogEngine, including networking, static scene definition using XML, scripting and sprite rendering.

* Network: shows synchronization of a rotated object
* Network2: shows transfering action messages between client and server
* Network3: synchronization of a large amount of objects
* Scenes: 6 static scenes in XML configuration files
* SceneSwitch: examples that switches between two scenes
* Scripting: switching the image using the script
* Scripting2: object movement by script
* Scripting3: message exchange between two scripts
* Sprites: rendering a large number of objects using the Sprite sheet renderer
* Transform: 20 scenes with various transformation settings

Examples that works with network must be run twice: once as a client and once as a server

## Next steps

This has been a brief introduction to get you up and running with CogEngine. A brief introduction into the architecture is [here](architecture.md) and documentation of the XML config is [here](xmlconfig.md).


[Home](Readme.md)

