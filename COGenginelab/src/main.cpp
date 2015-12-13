#include "ofMain.h"
#include "ofApp.h"
#include "Factory.h"
#include "Node.h"
#include "RotateAnim.h"
#include "TranslateAnim.h"
#include "HitEvent.h"
#include "Collider.h"

#include "duk_config.h"
#include "duktape.h"

long mstart;
long mend;
long temp;
using namespace std;
using namespace Cog;
int rotateAnimId;

void WriteTime(const char* msg) {
	cout << msg << " " << (ofGetElapsedTimeMillis() - temp) << " ms" << endl;
	temp = ofGetElapsedTimeMillis();
}

class JavaScriptBehavior : public Behavior {
	duk_context* ctx;
	
	string GetBehaviorJsName() {
		return "Behavior_" + ofToString(this->GetId());
	}

public:

	JavaScriptBehavior(duk_context* context) {
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

		if (duk_pnew(ctx, 1) != 0) {
			printf("%s\n", duk_safe_to_string(ctx, -1));
		}

		duk_put_prop_string(ctx, -2, GetBehaviorJsName().c_str());


		//string newObj = "var "+GetBehaviorJsName() + " = new " + "TestingBehavior" + "(" + ofToString(rotateAnimId)+");";
		//int isOK = duk_peval_string(ctx, newObj.c_str());
	}

	void Init() {
		RegisterListening(ACT_BEHAVIOR_FINISHED);
	}



	void OnMessage(Msg& msg) {

		duk_get_prop_string(ctx, -1, GetBehaviorJsName().c_str());
		duk_get_prop_string(ctx, -1, "OnMessage");
		duk_dup(ctx, -2);
		duk_push_number(ctx, msg.GetAction());
		duk_push_number(ctx, msg.GetBehaviorId());
		int isOK = duk_pcall_method(ctx, 2);

		if (isOK != 0) {
			printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}

		// pop function
		duk_pop(ctx);
		// pop instance
		duk_pop(ctx);



		//string onmsg = GetBehaviorJsName() + ".OnMessage(" + ofToString(msg.GetAction()) + "," + ofToString(msg.GetBehaviorId()) + ");";
		//int isOK = duk_peval_string(ctx, onmsg.c_str());
	}


	void Update(const uint64 delta, const uint64 absolute) {

		duk_get_prop_string(ctx, -1, GetBehaviorJsName().c_str());
		duk_get_prop_string(ctx, -1, "Update");
		duk_dup(ctx, -2);
		duk_push_number(ctx, delta);
		duk_push_number(ctx, absolute);
		int isOK = duk_pcall_method(ctx, 2);


		if (isOK != 0) {
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

class TestingBehavior : public Behavior {

public:

	TestingBehavior() {

	}

	void Init() {
		othersEnded = false;
		fpsCounter = 0;
		RegisterListening(ACT_BEHAVIOR_FINISHED);
	}

	int fpsCounter;
	bool othersEnded;

	void OnMessage(Msg& msg) {
		if (!othersEnded) {
			if (msg.GetAction() == ACT_BEHAVIOR_FINISHED && msg.GetBehaviorId() == rotateAnimId) {
				long mojo = temp;
				mend = ofGetElapsedTimeMillis();

				WriteTime("ANIM");
				cout << "FPS: " << ofToString(fpsCounter / ((ofGetElapsedTimeMillis() - mojo) / 1000)) << endl;
				cout << "TOTAL: " << ofToString(mend - mstart) << " ms" << endl;

				othersEnded = true;
			}
		}
	}


	void Update(const uint64 delta, const uint64 absolute) {
		fpsCounter++;
	}
};

int OnTestAnimFinished(duk_context *ctx) {
	int fpsCounter = duk_to_number(ctx, 0);

	long mojo = temp;
	mend = ofGetElapsedTimeMillis();

	WriteTime("ANIM");
	cout << "FPS: " << ofToString(fpsCounter / ((ofGetElapsedTimeMillis() - mojo) / 1000)) << endl;
	cout << "TOTAL: " << ofToString(mend - mstart) << " ms" << endl;


	return 1;
}

class TestingFactory : public Factory {

public:
	Node* CreateRoot() {

		ofLogNotice("test") << "vytvarim hlavni uzel";
		mstart = temp = ofGetElapsedTimeMillis();

		Node* root = new Node(ObjType::ROOT, 12, "root");

		ofLogNotice("test") << "nacitam barvicky";
		for (int i = 0; i < 2000; i++) {
			spt<ofImage> img = CogGet2DImage("images/blue.png");
			spt<ofImage> img2 = CogGet2DImage("images/red.png");

			Node* child = new Node("item");

			float rand1 = ofRandomf() / 2 + 0.5f;
			float rand2 = ofRandomf() / 2 + 0.5f;

			SetTransform(child, ofVec2f(rand1, rand2), CalcType::PER, 0.01f, CalcType::PER, ofVec2f(0.5f, 0.5f), 40, 40, root);


			if (i % 2 == 0 || true) {
				child->SetShape(spt<SpriteShape>(new SpriteShape(spt<Sprite>(new Sprite(spt<SpriteSet>(new SpriteSet(
					new SpriteSheet("mojo", img), 0, 0, 1, 256, 256, 256, 256, 256, 256)), 0)))));
			}
			else {
				child->SetShape(spt<Image>(new Image(img2)));
			}


			float scale = child->GetTransform().scale.x;
			child->GetTransform().rotationOrigin = ofVec2f((CogGetScreenWidth() / 2 - child->GetTransform().absPos.x) / scale,
				(CogGetScreenHeight() / 2 - child->GetTransform().absPos.y) / scale);

			RotateAnim* anim = new RotateAnim(0, 360, 0.8f, false);
			rotateAnimId = anim->GetId();
			child->AddBehavior(anim);
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 50, rand2 * 100), 0.1f, true));
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 20, rand2 * 12), 0.1f, true));
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 80, -rand2 * 5), 0.1f, true));
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 40, rand2 * 80), 0.1f, true));
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(rand1 * 30, -rand2 * 60), 0.1f, true));
			child->AddBehavior(new TranslateAnim(ofVec3f(0, 0, 0), ofVec3f(-rand1 * 20, -rand2 * 2), 0.1f, true));
			if (i % 2 == 0) child->AddBehavior(new HitEvent(0, false));
			if (i % 50 == 0) child->SetGroup(12345);
			root->AddChild(child);
		}

		WriteTime("INIT");

		ofLogNotice("test") << "zapisuju ze je vsechno OK";

		root->AddBehavior(new Collider(12345));
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

		ofLogNotice("test") << "spoustim barvicky";
		return root;
	}
};

int mojo(duk_context *ctx) {
	int argument = duk_to_number(ctx, 0);
	ofLog(OF_LOG_NOTICE, "mojo " + ofToString(argument));
	cout << "MOJO :: " << argument << endl;
	return 1;
}

int dojo(duk_context *ctx) {
	int argument = duk_to_number(ctx, 0);
	cout << "DOJO :: " << argument << endl;
	ofLog(OF_LOG_NOTICE, "DOJO " + ofToString(argument));
	return 0;
}


#ifdef WIN32
#ifdef TESTING

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"

struct TestPointer
{
	TestPointer(const int data) : Data(data) {}
	TestPointer(const TestPointer & other)
	{
		Data = other.Data;
		++CopyConstructorCount;
	}

	~TestPointer()
	{
		++DestructorCount;
	}

	int Data;

	static size_t CopyConstructorCount;
	static size_t DestructorCount;
};

size_t TestPointer::CopyConstructorCount = 0;
size_t TestPointer::DestructorCount = 0;

dukbind_bind_as_raw_pointer(TestPointer)

static bool DoStuffIsCalled = false;

static duk_ret_t DoStuff(duk_context *)
{
	DoStuffIsCalled = true;
	return 0;
}

static duk_ret_t CheckThis(duk_context * ctx)
{
	duk_push_this(ctx);

	const TestPointer & result = dukbind::Get(ctx, -1, (TestPointer*)0);

	REQUIRE(result.Data == 5678);

	return 0;
}

TEST_CASE("Class can be passed as pointer", "[binding][class]")
{
	duk_context * ctx = duk_create_heap_default();

	dukbind::BindingInfo info;

	info.AddClass("TestPointer", dukbind::rtti::GetTypeIndex<TestPointer>());
	info.AddMethod(dukbind::rtti::GetTypeIndex<TestPointer>(), "DoStuff", DoStuff);
	info.AddMethod(dukbind::rtti::GetTypeIndex<TestPointer>(), "CheckThis", CheckThis);

	dukbind::Setup(ctx, info, "Module");

	SECTION("Instance is not copied when pushed")
	{
		TestPointer data(1234);
		TestPointer::CopyConstructorCount = 0;
		dukbind::Push(ctx, data, (TestPointer*)0);
		REQUIRE(TestPointer::CopyConstructorCount == 0);
	}

	SECTION("Instance is not copied when pushed")
	{
		TestPointer data(5678);

		dukbind::Push(ctx, data, (TestPointer*)0);

		const TestPointer & result = dukbind::Get(ctx, -1, (TestPointer*)0);

		REQUIRE(&result == &data);
	}

	SECTION("Instance is not destructed when garbage collected")
	{
		TestPointer data(1234);
		dukbind::Push(ctx, data, (TestPointer*)0);
		TestPointer::DestructorCount = 0;

		duk_pop(ctx);
		duk_gc(ctx, 0);
		duk_gc(ctx, 0);
		REQUIRE(TestPointer::DestructorCount == 0);
	}

	SECTION("Binding is called")
	{
		TestPointer data(1234);
		duk_push_global_object(ctx);
		dukbind::Push(ctx, data, (TestPointer*)0);
		duk_put_prop_string(ctx, -2, "data");

		duk_eval_string_noresult(ctx, "data.DoStuff()");

		REQUIRE(DoStuffIsCalled);
	}

	SECTION("This is valid")
	{
		TestPointer data(5678);
		duk_push_global_object(ctx);
		dukbind::Push(ctx, data, (TestPointer*)0);
		duk_put_prop_string(ctx, -2, "data");

		duk_eval_string_noresult(ctx, "data.CheckThis()");
	}

	duk_destroy_heap(ctx);
}





#else

#include "Engine.h"

class TestingApp : public CogApp {

	TestingFactory* fact;

	void InitComponents() {
		fact = new TestingFactory();
		COGEngine.componentStorage->RegisterComponent(fact);
	}

	void InitEngine() {
		spt<ofxXmlSettings> config;

		COGEngine.Init();
		COGEngine.nodeStorage->SetRootObject(fact->CreateRoot());
	}
};

class XmlTestingApp : public CogApp {

	
	void InitComponents() {

	}

	void InitEngine() {
		ofxXmlSettings* xml = new ofxXmlSettings();
		xml->loadFile("configexample.xml");
		auto xmlPtr = spt<ofxXmlSettings>(xml);

		COGEngine.Init(xmlPtr);

		xmlPtr->popAll();
		xmlPtr->pushTag("app_config");
		xmlPtr->pushTag("scenes");
		xmlPtr->pushTag("scene", 0);

		auto mgr = GETCOMPONENT(NodeStorage);
		mgr->LoadSceneFromXml(xmlPtr);
	}
};


int main() {

	duk_context *ctx = duk_create_heap_default();

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, mojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "mojo");

	duk_push_c_function(ctx, dojo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2 /*idx:global*/, "dojo");
	duk_pop(ctx);

	int output = 0;

	output = duk_peval_string(ctx, "var bobo = 25; var blbost = 'ahoj nazdar';  bobo = Math.min(0,5);");

	if (output != 0) {
		string err = duk_safe_to_string(ctx, -1);
	}

	output = duk_peval_string(ctx, "mojo(bobo);");

	//duk_eval_string(ctx, "(dojo(12));");

	//duk_pop(ctx);  /* pop global */


	//duk_destroy_heap(ctx);

	//ofAppGLFWWindow window;
	//ofSetupOpenGL(&window, 225,400,OF_WINDOW);

	ofSetupOpenGL(800, 450, OF_WINDOW);
	//window.setDoubleBuffering(true);
	//window.setNumSamples(4);

	//window.setGlutDisplayString("rgba double samples=4 depth");
	//window.setWindowTitle("COGEngine");
	//ofRunApp(new MTestApp());
	
	//ofRunApp(new TestingApp());
	ofRunApp(new XmlTestingApp());



	return 0;
}
#endif

#else
#include <jni.h>

int main() {

	ofLogNotice("test") << "spustil jsem aplikaci";
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

	ofLogNotice("test") << "nacitam ruzne atributy";

	duk_destroy_heap(ctx);

	ofSetupOpenGL(720, 1280, OF_WINDOW);			// <-------- setup the GL context
	cout << "Android app loaded" << endl;
	//ofRunApp(new MTestApp());
	ofLogNotice("test") << "spoustim appku s testingFactory";
	ofRunApp(new App(new TestingFactory()));
	return 0;
}


//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif

