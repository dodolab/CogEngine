#include "ofxCogMain.h"
#include "AnimationLoader.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"

class XmlTestingApp : public CogApp {


	void InitComponents() {

	}

	void InitEngine() {
		ofxXml* xml = new ofxXml();
		/*xml->loadFile("config.xml");
		auto xmlPtr = spt<ofxXml>(xml);
		
		COGEngine.Init(xmlPtr);*/

		COGEngine.Init();

		auto resCache = GETCOMPONENT(ResourceCache);

		// <animations>
		auto anims = vector<spt<Anim>>();
		spt<Anim> anim = spt<Anim>(new Anim("square_anim", "sheet_squares.png", "", 4, 2, 0, 3, 1, 0.2, 0, false));
		resCache->StoreAnimation(anim);
		
		// <spritesheets>
		spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet("squares",CogGet2DImage("sheet_squares.png")));
		spriteSheet->AddSpriteSet("squares", 4, 128, 128);
		resCache->StoreSpriteSheet(spriteSheet);

		// <transforms>
		spt<TransformEnt> trans = spt<TransformEnt>(new TransformEnt("rot_begin", ofVec2f(0), 10, CalcType::LOC, ofVec2f(0), ofVec2f(1), CalcType::LOC, 0));
		spt<TransformEnt> trans2 = spt<TransformEnt>(new TransformEnt("rot_end", ofVec2f(0), 10, CalcType::LOC, ofVec2f(0), ofVec2f(1), CalcType::LOC, 360));
		spt<TransformEnt> trans3 = spt<TransformEnt>(new TransformEnt("scale_begin", ofVec2f(0), 10, CalcType::LOC, ofVec2f(0), ofVec2f(1), CalcType::LOC, 0));
		spt<TransformEnt> trans4 = spt<TransformEnt>(new TransformEnt("scale_end", ofVec2f(0), 10, CalcType::LOC, ofVec2f(0), ofVec2f(-1), CalcType::LOC, 0));
		resCache->StoreEntity(trans);
		resCache->StoreEntity(trans2);
		resCache->StoreEntity(trans3);
		resCache->StoreEntity(trans4);

		// <behaviors>
		Setting setting = Setting();
		setting.AddItem("name", "transform1");
		setting.AddItem("type", "TransformAnim");
		setting.AddItem("from", "rot_begin");
		setting.AddItem("to", "rot_end");
		setting.AddItem("duration", "4000");
		setting.AddItem("repeat", "false");
		setting.AddItem("blend", "additive");
		spt<BehaviorEnt> beh1 = spt<BehaviorEnt>(new BehaviorEnt("transform1", setting));
		Setting setting2 = Setting();
		setting2.AddItem("name", "transform1");
		setting2.AddItem("type", "TransformAnim");
		setting2.AddItem("from", "scale_begin");
		setting2.AddItem("to", "scale_end");
		setting2.AddItem("duration", "4000");
		setting2.AddItem("repeat", "false");
		setting2.AddItem("blend", "additive");
		spt<BehaviorEnt> beh2 = spt<BehaviorEnt>(new BehaviorEnt("transform2", setting2));
		Setting setting3 = Setting();
		setting3.AddItem("name", "transform1");
		setting3.AddItem("type", "TransformAnim");
		setting3.AddItem("animation", "square_anim");
		spt<BehaviorEnt> beh3 = spt<BehaviorEnt>(new BehaviorEnt("squareanim", setting3));
		resCache->StoreEntity(beh1);
		resCache->StoreEntity(beh2);
		resCache->StoreEntity(beh3);

		// <scenes>
		Scene* main = new Scene();
		// todo ???
		main->LoadInitData("main", false, 0);
		LayerEnt layer1 = LayerEnt("bgr", "bgr", 100, 30);
		LayerEnt layer2 = LayerEnt("squares", "squares", 100, 30);
		main->AddLayer(layer1);
		main->AddLayer(layer2);

		NodeBuilder bld = NodeBuilder();
		Node* node1 = new Node("bgr");
		auto spriteSheet1 = resCache->GetSpriteSheet("bgr");
		// todo ???
		node1->SetShape(spt<SpriteShape>(new SpriteShape(spt<Sprite>(new Sprite(spriteSheet1->GetDefaultSpriteSet(), 0)),"bgr")));
		main->AddNode(node1);

		Node* node2 = new Node("square_2");


		auto context = GETCOMPONENT(SceneContext);
		context->AddScene(main, true);
	}
};


 
int main() {

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new XmlTestingApp());

	return 0;
}


