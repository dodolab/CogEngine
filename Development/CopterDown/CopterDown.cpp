#include "s3e.h"
#include "IwDebug.h"
#include "Iw2DSceneGraph.h"
#include "IwGx.h"
#include "Gnode.h"
#include "unistd.h"
#include <iostream>
#include "EnUserAct.h"
#include "SmartPointer.h"
#include "BeRender.h"
#include "BeTranslateAnim.h"

using namespace Iw2DSceneGraphCore;



// ============================== VARIABLES

// flag for screen size change
bool screenSizeChanged = true;


int width;
int height;
// ============================== LOOPS
GNode* root;
uint64 absolute;
spt<EnUserAct> userActions;
CIwResGroup* resourceGroup;

void UpdateInputs(){
	// todo: add new inputs to the root attribute
}

void CheckInputs(){
	auto actions = userActions->GetKeyActions();
	auto points = userActions->GetPointActions();

	for (auto it = actions.begin(); it != actions.end(); /*nothing*/){
		EnInputAct<Act>& act = (*it);
		if (act.ended || act.handled){
			actions.erase(it);
		}
		else{
			act.cycleNumber++;
			++it;
		}
	}

	for (auto it = points.begin(); it != points.end(); /*nothing*/){
		EnInputAct<CIwFVec2> act = (*it);
		if (act.ended || act.handled){
			points.erase(it);
		}
		else{
			act.cycleNumber++;
			++it;
		}
	}
}

void Update(uint64 delta, uint64 absolute){
	root->Update(delta, absolute, CIwFMat2D::g_Identity.SetTrans(CIwFVec2(width / 2, height)));
}

void Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	Iw2DSurfaceClear(0xff000000);

	// draw the root node
	root->Draw(delta, absolute, CIwFMat2D::g_Identity);

	// Show the drawing surface
	Iw2DSurfaceShow();
}

void StartLoop(){

	uint64 deltaTime = s3eTimerGetMs();
	double angle = 0;

	while (!s3eDeviceCheckQuitRequest())
	{
		uint64 delta = s3eTimerGetMs() - deltaTime;
		deltaTime = s3eTimerGetMs();

		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();		
		UpdateInputs();

		Update(delta, absolute);
		Draw(delta, absolute);

		CheckInputs();

		// sleep 0ms (for input events update)
		s3eDeviceYield(0);
	}
}

// ============================== HANDLERS

// Callback from S3E when the screen size or rotation changes
int32 ScreenSizeChangeCallback(void* systemData, void* userData)
{
	screenSizeChanged = true;
	return 0;
}

vector<s3eKey> pressedKeys;

Act GetAction(s3eKey key){
	if (key == s3eKeyLeft) return Act::LEFT;
	if (key == s3eKeyRight) return Act::RIGHT;
	if (key == s3eKeySpace) return Act::FIRE;
	if (key == s3eKeyTab) return Act::SWITCH;
	return Act::FIRE;
}

int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData){
	
	if (event->m_Pressed){
		// key down


		// if pressed keys contains this key, do nothing
		for (auto key : pressedKeys){
			if (key == event->m_Key) return 0;
		}

		pressedKeys.push_back(event->m_Key);
		Act inAct = GetAction(event->m_Key);

		bool alreadyContains = false;
		for (EnInputAct<Act> act : userActions->GetKeyActions()){
			if (act.value == inAct){
				alreadyContains = true;
				break;
			}
		}

		if (!alreadyContains) userActions->GetKeyActions().push_back(EnInputAct<Act>(inAct));
	}
	else{
		// key up
		for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it){
			if ((*it) == event->m_Key){
				pressedKeys.erase(it);
				break;
			}
		}

		Act inAct = GetAction(event->m_Key);

		for (EnInputAct<Act> act : userActions->GetKeyActions()){
			if (act.value == inAct){
				act.ended = true;
				break;
			}
		}
	}

	return 0;
}

int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData){
	return 0; 
}

// handled even when mouse button is not pressed !!
int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData){
	return 0;
}

// ============================== INITIALIZATION

void RegisterEvents(){
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, ScreenSizeChangeCallback, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PointerButtonEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PointerMotionEventCallback, NULL);
}


void Init(){

	// initialize 2D graphic system
	Iw2DInit();
	// initialize common graphic system
	IwGxInit();
	// initialize resources manager
	IwResManagerInit();
	// dont use mipmaps -> better memory usage
	Iw2DSetUseMipMapping(false);
	
	height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	width = height = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);

	// initialize root node
	root = new GNode(ObjType::ROOT, 0, "root");
	userActions = spt<EnUserAct>(new EnUserAct());
	root->AddAttr(Attrs::USERACTION, userActions);
	
	// load application data
	resourceGroup = IwGetResManager()->LoadGroup("resources.group");

	// Load images
	spt<CIw2DImage> image = spt<CIw2DImage>(Iw2DCreateImageResource("blue"));

	for (int i = 0; i < 2000; i++){
		GNode* child = new GNode(ObjType::OBJECT, 0, "other");

		CIwFVec2 randomTransform(IwRandMinMax(1, width), IwRandMinMax(1, height*2));
		CIwFVec2 randomTransform2(IwRandMinMax(1, width), IwRandMinMax(1, height*2));

		child->GetTransform().SetTrans(randomTransform);
		child->GetTransform().Scale(0.063f);

		child->AddAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE, image);
		child->AddBehavior(new BeRender());
		float speed = 0.1f*((float)IwRandMinMax(1, 15));

		child->AddBehavior(new BeTranslateAnim(randomTransform, randomTransform2, speed, true, true));

		root->AddChild(child);
	}
}


void Terminate(){
	// terminate all used modules
	IwResManagerTerminate();
	Iw2DTerminate();
	IwGxTerminate();

}

// ============================== MAIN METHOD


using namespace std;
class TestM{

public:
	int mojo;

	TestM(){
		s3eDebugOutputString("Constructor");
	}

	TestM(int mojo){
		s3eDebugOutputString("Constructor");
		this->mojo = mojo;
	}

	~TestM(){
		s3eDebugOutputString("Destructor");
	}

	TestM(const TestM& copy){
		mojo = copy.mojo;
		s3eDebugOutputString("Copy");
	}
};





void test(){
	// enflags test
	EnFlags flags;
	flags.SetState(1);
	flags.SetState(1234);
	bool has = flags.HasState(1);
	has = flags.HasState(1234);
	flags.ResetState(1);
	flags.ResetState(1234);
	has = flags.HasState(1);
	has = flags.HasState(1234);

	flags = 12;
	flags = 13;
	
/*	node = new GNode(ObjType::OBJECT, 12, "fofka");
	node->AddAttr(12, 12);

	int* pointer = new int(4);
	node->AddAttr(13, pointer);

	int mojo = 5;
	node->AddAttr(6, mojo);
	*/
//	TestM testik(12);
//	node->AddAttr(10, ElemType::ALL, testik);

	spt<TestM> testik2 = spt<TestM>(new TestM(555));
	TestM* testik3 = new TestM(123);

	int sizeOfObject = sizeof(GNode);

	//TestM& testikPuvodni = node->FindAttr<TestM>(10);

//	spt<TestM> testikPointer = node->FindAttr<spt<TestM>>(11);
//	node->RemoveAttr(11);

	auto actual = s3eTimerGetMs();
	for (int i = 0; i < 100000; i++){
//		node->AddAttr(11, testik3);
		//TestM* testf = node->GetAttr<TestM*>(11);
	//	node->RemoveAttr(11);
	}

	auto diff = s3eTimerGetMs() - actual;
}

int main()
{
	absolute = s3eTimerGetMs();
	test();

	
	// initialize
	Init();
	// register input events
	RegisterEvents();
	// start drawing loop
	StartLoop();
	// finish
	Terminate();

	return 0;
}
