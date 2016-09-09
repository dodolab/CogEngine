#include "ofMain.h"
#include "ofApp.h"
#include "ofxArcFactory.h"
#include "ofxAreNode.h"
#include "ofxArbRotateAnim.h"
#include "ofxArbTranslateAnim.h"
#include "ofxArbHitEvent.h"
#include "ofxArbCollider.h"

long mstart;
long mend;
long temp;
using namespace std;
int rotateAnimId;

void WriteTime(const char* msg){
	cout << msg << " " << (ofGetElapsedTimeMillis() - temp) << " ms" << endl;
	temp = ofGetElapsedTimeMillis();
}

class TestingBehavior : public ofxAreBehavior{

public:

	TestingBehavior()  {

	}

	void Init(){
		othersEnded = false;
		fpsCounter = 0;
		RegisterListening(Actions::BEHAVIOR_FINISHED);
	}

	int fpsCounter;
	bool othersEnded;

	void OnMessage(ofxAreMsg& msg){
		if (!othersEnded){
			if (msg.GetAction() == Actions::BEHAVIOR_FINISHED && msg.GetBehaviorId() == rotateAnimId){
				long mojo = temp;
				mend = ofGetElapsedTimeMillis();

				WriteTime("ANIM");
				cout << "FPS: " << ofToString(fpsCounter / ((ofGetElapsedTimeMillis() - mojo) / 1000)) << endl;
				cout << "TOTAL: " << ofToString(mend - mstart) << " ms" << endl;

				othersEnded = true;
			}
		}
	}


	void Update(const uint64 delta, const uint64 absolute){
		fpsCounter++;
	}
};


class TestingFactory : public ofxArcFactory{
	
public:
	ofxAreNode* CreateRoot(){

		mstart = temp = ofGetElapsedTimeMillis();

		ofxAreNode* root = new ofxAreNode(ObjType::ROOT, 12, "root");

		for (int i = 0; i < 2000; i++){
			spt<ofImage> img = COGGet2DImage("images/blue.png");
			spt<ofImage> img2 = COGGet2DImage("images/red.png");

			ofxAreNode* child = new ofxAreNode("item");

			float rand1 = ofRandomf()/2 + 0.5f;
			float rand2 = ofRandomf()/2 + 0.5f;

			SetTransform(child, ofVec2f(rand1, rand2), CalcType::PER, 0.01f, CalcType::PER, ofVec2f(0.5f, 0.5f), 40, 40, root);
			

			if (i % 2 == 0 || true){
				child->SetShape(spt<ofxAraSpriteShape>(new ofxAraSpriteShape(spt<ofxAraSprite>(new ofxAraSprite(spt<ofxAraSpriteSet>(new ofxAraSpriteSet(
					new ofxAraSpriteSheet("mojo",img),0,0,1,256,256,256,256,256,256)),0)))));
			}
			else{
				child->SetShape(spt<EnImageShape>(new EnImageShape(img2)));
			}


			float scale = child->GetTransform().scale.x;
			child->GetTransform().rotationOrigin = ofVec2f((COGGetScreenWidth() / 2 - child->GetTransform().absPos.x) / scale, 
				(COGGetScreenHeight() / 2 - child->GetTransform().absPos.y) / scale);

			ofxArbRotateAnim* anim = new ofxArbRotateAnim(0, 360, 0.8f, false);
			rotateAnimId = anim->GetId();
			child->AddBehavior(anim);
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 50, rand2 * 100), 0.1f, true));
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 20, rand2 * 12), 0.1f, true));
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 80, -rand2 * 5), 0.1f, true));
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 40, rand2 * 80), 0.1f, true));
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 30, -rand2 * 60), 0.1f, true));
			child->AddBehavior(new ofxArbTranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 20, -rand2 * 2), 0.1f, true));
			if(i%2 == 0) child->AddBehavior(new ofxArbHitEvent(0, false));
			if(i%50 == 0) child->SetGroup(12345);
			root->AddChild(child);
		}
		
		WriteTime("INIT");

		root->AddBehavior(new ofxArbCollider(12345));
		root->AddBehavior(new TestingBehavior());
		root->SubmitChanges(true);

		WriteTime("SUBMIT CHANGES");

		return root;
	}
};



#ifdef TARGET_WINDOWS
int main(){
	//ofAppGLFWWindow window;
	//ofSetupOpenGL(&window, 225,400,OF_WINDOW);
	ofSetupOpenGL(450, 800, OF_WINDOW);
	//window.setDoubleBuffering(true);
	//window.setNumSamples(4);

	//window.setGlutDisplayString("rgba double samples=4 depth");
	//window.setWindowTitle("COGEngine");
	ofRunApp(new MTestApp());
	//ofRunApp(new MApp(new TestingFactory()));
	
	return 0;
}

#else
#include <jni.h>

int main(){
	ofSetupOpenGL(720, 1280, OF_WINDOW);			// <-------- setup the GL context
	cout << "Android app loaded" << endl;
	//ofRunApp(new MTestApp());
	ofRunApp(new ofxAreApp(new TestingFactory()));
	return 0;
}


//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz){
		main();
	}
}
#endif

