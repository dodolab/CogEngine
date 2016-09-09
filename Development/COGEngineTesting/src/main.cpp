#include "ofMain.h"
#include "ofApp.h"
#include "ofxArcFactory.h"
#include "ofxAreNode.h"
#include "ofxArbRotateAnim.h"
#include "ofxArbTranslateAnim.h"
#include "ofxArbHitEvent.h"
#include "ofxArbCollider.h"

#include "duk_config.h"
#include "duktape.h"

long mstart;
long mend;
long temp;
using namespace std;
int rotateAnimId;

void WriteTime(const char* msg){
	cout << msg << " " << (ofGetElapsedTimeMillis() - temp) << " ms" << endl;
	temp = ofGetElapsedTimeMillis();
}

class JavaScriptBehavior : public ofxAreBehavior{
	duk_context* ctx;

	string GetBehaviorJsName(){
		return "Behavior_"+ofToString(this->GetId());
	}

public:

	JavaScriptBehavior(duk_context* context){
		this->ctx = context;
		/*
		string path = ofToDataPath("scriptTest.js");

		string str, strTotal;
		ifstream in;
		in.open(path);
		getline(in, str);
		while (in) {
		strTotal += str+"\n";
		getline(in, str);
		}

		//	duk_peval_string(ctx, strTotal.c_str());
		if (duk_peval_string(ctx, strTotal.c_str()) != 0) {
		printf("compile failed: %s\n", duk_safe_to_string(ctx, -1));
		*/

		if (duk_peval_file(ctx, ofToDataPath("scriptTest.js").c_str()) != 0) {
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		}
		duk_pop(ctx);  /* ignore result */

		
		duk_push_global_object(ctx);
		duk_get_global_string(ctx, "TestingBehavior");
		duk_push_int(ctx, rotateAnimId);
		
		if (duk_pnew(ctx, 1) != 0){
			printf("%s\n", duk_safe_to_string(ctx, -1));
		}

		duk_put_prop_string(ctx, -2, GetBehaviorJsName().c_str());


		//string newObj = "var "+GetBehaviorJsName() + " = new " + "TestingBehavior" + "(" + ofToString(rotateAnimId)+");";
		//int isOK = duk_peval_string(ctx, newObj.c_str());
	}

	void Init(){
		RegisterListening(Actions::BEHAVIOR_FINISHED);
	}


	
	void OnMessage(ofxAreMsg& msg){
		
		duk_get_prop_string(ctx, -1, GetBehaviorJsName().c_str());
		duk_get_prop_string(ctx, -1, "OnMessage");
		duk_dup(ctx, -2);
		duk_push_number(ctx, msg.GetAction());
		duk_push_number(ctx, msg.GetBehaviorId());
		int isOK = duk_pcall_method(ctx, 2);

		if (isOK != 0){
			printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}

		// pop function
		duk_pop(ctx);
		// pop instance
		duk_pop(ctx);



		//string onmsg = GetBehaviorJsName() + ".OnMessage(" + ofToString(msg.GetAction()) + "," + ofToString(msg.GetBehaviorId()) + ");";
		//int isOK = duk_peval_string(ctx, onmsg.c_str());
	}


	void Update(const uint64 delta, const uint64 absolute){

		duk_get_prop_string(ctx, -1, GetBehaviorJsName().c_str());
		duk_get_prop_string(ctx, -1, "Update");
		duk_dup(ctx, -2);
		duk_push_number(ctx, delta);
		duk_push_number(ctx, absolute);
		int isOK = duk_pcall_method(ctx, 2);


		if (isOK != 0){
			printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}

		// pop function
		duk_pop(ctx);
		// pop instance
		duk_pop(ctx);


		//string updateObj = GetBehaviorJsName() + ".Update(" + ofToString(delta) + "," + ofToString(absolute) + ");";
		//int isOK = duk_peval_string(ctx, updateObj.c_str());

	}
};

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

int OnTestAnimFinished(duk_context *ctx){
	int fpsCounter = duk_to_number(ctx, 0);

	long mojo = temp;
	mend = ofGetElapsedTimeMillis();

	WriteTime("ANIM");
	cout << "FPS: " << ofToString(fpsCounter / ((ofGetElapsedTimeMillis() - mojo) / 1000)) << endl;
	cout << "TOTAL: " << ofToString(mend - mstart) << " ms" << endl;


	return 1;
}

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
		//root->AddBehavior(new TestingBehavior());
		duk_context *ctx = duk_create_heap_default();
		duk_push_global_object(ctx);
		duk_push_c_function(ctx, OnTestAnimFinished, DUK_VARARGS);
		duk_put_prop_string(ctx, -2 /*idx:global*/, "OnTestAnimFinished");
		duk_pop(ctx);


		JavaScriptBehavior* jsbeh = new JavaScriptBehavior(ctx);
		root->AddBehavior(jsbeh);

		
		root->SubmitChanges(true);

		WriteTime("SUBMIT CHANGES");

		return root;
	}
};

int mojo(duk_context *ctx){
	int argument = duk_to_number(ctx, 0);
	ofLog(OF_LOG_NOTICE, "mojo " + ofToString(argument));
	cout << "MOJO :: " << argument << endl;
	return 1;
}

int dojo(duk_context *ctx){
	int argument = duk_to_number(ctx, 0);
	cout << "DOJO :: " << argument << endl;
	ofLog(OF_LOG_NOTICE, "DOJO " + ofToString(argument));
	return 0;
}


#ifdef TARGET_WINDOWS
int main(){

	duk_context *ctx = duk_create_heap_default();
	
	duk_push_global_object(ctx);
	duk_push_c_function(ctx, mojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "mojo");
	
	duk_push_c_function(ctx, dojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "dojo");
	duk_pop(ctx);

	int output = 0;

	output = duk_peval_string(ctx, "var bobo = 25; var blbost = 'ahoj nazdar';  bobo = Math.min(0,5);");
	
	if (output != 0){
		string err = duk_safe_to_string(ctx, -1);
	}
	
	output = duk_peval_string(ctx, "mojo(bobo);");

	//duk_eval_string(ctx, "(dojo(12));");

	//duk_pop(ctx);  /* pop global */


	//duk_destroy_heap(ctx);

	//ofAppGLFWWindow window;
	//ofSetupOpenGL(&window, 225,400,OF_WINDOW);
	ofSetupOpenGL(450, 800, OF_WINDOW);
	//window.setDoubleBuffering(true);
	//window.setNumSamples(4);

	//window.setGlutDisplayString("rgba double samples=4 depth");
	//window.setWindowTitle("COGEngine");
	//ofRunApp(new MTestApp());
	ofRunApp(new ofxAreApp(new TestingFactory()));
	
	return 0;
}

#else
#include <jni.h>

int main(){

	duk_context *ctx = duk_create_heap_default();

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, mojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "mojo");

	duk_push_c_function(ctx, dojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "dojo");
	duk_pop(ctx);

	duk_eval_string(ctx, "dojo(mojo(12));");
	//duk_eval_string(ctx, "(dojo(12));");

	//duk_pop(ctx);  /* pop global */


	duk_destroy_heap(ctx);

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

