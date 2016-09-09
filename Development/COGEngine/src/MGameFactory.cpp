#include "MGameFactory.h"
#include "SmartPointer.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "BeHitEvent.h"

GNode* CreateFirst(){
	GNode* output = new GNode(ObjType::SCENE, 1, "first");

	// Load images
	spt<ofImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");
	spt<ofImage> rytmus = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");

	int pixels = MEngine.environmentCtrl->GetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* actual = output;
	ofColor color;
	Vectorf3 actualSize = Vectorf3(300, 300);

	for (int i = 0; i < 1; i++){
		color.set(10 + i * 4, 100 + i * 20, 0);
		GNode* child = new GNode(ObjType::OBJECT, 0, "fofka");
		child->GetTransform().LocalPos = actualSize;
		if (i == 0){
			child->GetTransform().LocalPos = Vectorf3(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2);
			child->GetTransform().Scale =  Vectorf3(scale * 30,scale * 30,scale * 30);
			child->AddAttr(Attrs::IMGSOURCE, rytmus);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent());
			child->AddBehavior(new BeRender(RenderType::IMAGE));
			child->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		}

		/*	child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		child->AddBehavior(new BeRender(RenderType::RECTANGLE));
		child->AddBehavior(new BeRotateAnim(0, 0, 2, false));*/
		actual->AddChild(child);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		actual = child;
		actualSize = actualSize / 2;
	}

	return output;
}

GNode* CreateSecond(){
	GNode* output = new GNode(ObjType::SCENE, 2, "second");

	int pixels = MEngine.environmentCtrl->GetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* actual = output;
	ofColor color;
	Vectorf3 actualSize = Vectorf3(300, 300);

	for (int i = 0; i < 5; i++){
		color.set(10 + i * 4, 100 + i * 20, 0);
		GNode* child = new GNode(ObjType::OBJECT, 0, "fofka");
		child->GetTransform().LocalPos = actualSize;
		if (i == 0){
			child->GetTransform().LocalPos = Vectorf3(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2);
			child->GetTransform().Scale = Vectorf3(scale * 30, scale * 30, scale * 30);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent());
		}
		child->AddBehavior(new BeRender(RenderType::RECTANGLE));
		child->AddBehavior(new BeRotateAnim(0, 0, 12, false));

		child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		actual->AddChild(child);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale =Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = Vectorf3(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = Vectorf3(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		actual = child;
		actualSize = actualSize / 2;
	}

	return output;
}

GNode* CreateThird(){

	GNode* output = new GNode(ObjType::SCENE, 3, "sthird");
	spt<ofImage> rytmus = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");
	spt<ofImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");

	int pixels = MEngine.environmentCtrl->GetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* child = new GNode(ObjType::OBJECT, 10, "other");
	child->GetTransform().LocalPos = Vectorf3(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2);
	child->GetTransform().Scale = Vectorf3(scale * 10, scale * 10, scale * 10);
	child->AddAttr(Attrs::IMGSOURCE, rytmus);
	child->SetState(States::HITTABLE);
	child->AddBehavior(new BeHitEvent());
	child->AddBehavior(new BeRender(RenderType::IMAGE));
	child->AddBehavior(new BeRotateAnim(0, 0, 20, false));

	output->AddChild(child);

	for (int i = 0; i < 2000; i++){
		GNode* particle = new GNode(ObjType::OBJECT, 0, "other");

		Vectorf3 randomTransform(ofRandom(1, MEngine.environmentCtrl->GetWidth()), ofRandom(MEngine.environmentCtrl->GetHeight() / 2 - MEngine.environmentCtrl->GetWidth() / 2,
			MEngine.environmentCtrl->GetHeight() / 2 + MEngine.environmentCtrl->GetWidth() / 2));

		particle->GetTransform().LocalPos = randomTransform;
		particle->GetTransform().Scale = Vectorf3(scale * 2, scale * 2, scale * 2);
		particle->GetTransform().RotationOrigin = Vectorf3(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2) - particle->GetTransform().LocalPos;
		particle->AddAttr<spt<ofImage>>(Attrs::IMGSOURCE, image);
	
		ofColor color;
		color.set(0,0,222);
		particle->AddAttr(Attrs::COLOR, color);
		particle->AddAttr(Attrs::SIZE, Vectorf3(250,250));
		particle->AddBehavior(new BeRender(RenderType::IMAGE));
		float speed = particle->GetTransform().RotationOrigin.length() / 6;
		BeRotateAnim* rot = new BeRotateAnim(0, 0, speed, false);
		particle->AddBehavior(rot);

		output->AddChild(particle);

	}

	return output;
}

GNode* CreateMask(int width, int height, Vectorf3 position){
	GNode* mask = new GNode(ObjType::OBJECT, 0, "maska");
	mask->AddBehavior(new BeRender(RenderType::RECTANGLE));
	ofColor color;
	color.set(0, 0, 0);
	mask->AddAttr(Attrs::COLOR, color);
	mask->AddAttr(Attrs::SIZE, Vectorf3(width,height));
	mask->GetTransform().LocalPos = position;

	return mask;
}






Vectorf3 MGameFactory::CalcScale(spt<ofImage> img, float width, GNode* scene){
	int screenWidth = MEngine.environmentCtrl->GetWidth();

	// width is always 100 units !
	float rel = width/100.0f;

	float absoluteWidth = rel*screenWidth;

	float otp= ((absoluteWidth/img->width))/scene->GetTransform().Scale.x;
	return Vectorf3(otp,otp,otp);
}

float MGameFactory::RelPosX(float posX, GNode* scene){
	int screenWidth = MEngine.environmentCtrl->GetWidth();

	// width is always 100 units !
	float rel = posX/100.0f;

	return (rel*screenWidth-screenWidth/2)/scene->GetTransform().Scale.x;
}

float MGameFactory::RelPosY(float posY, GNode* scene){
	int screenHeight = MEngine.environmentCtrl->GetHeight();

	// width is always 60 units !
	float rel = posY/60.0f;

	return (rel*screenHeight-screenHeight/2)/scene->GetTransform().Scale.x;
}


void MGameFactory::SetTransform(GNode* node, float posX, float posY, float scale){
	node->GetTransform().LocalPos = Vectorf3(posX, posY);
	node->GetTransform().Scale = Vectorf3(scale,scale,scale);
}

// TODO !!!!! create Renderable entity with ANCHORS

void MGameFactory::SetRenderImage(GNode* node, string imgPath, bool isHittable){
	node->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	if(isHittable) node->SetState(States::HITTABLE);
}


void MGameFactory::SetSingleBackground(GNode* node, string imgPath, bool isHittable){
	SetRenderImage(node, imgPath, isHittable);
	spt<ofImage> img = node->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
	float scale = ((float)MEngine.environmentCtrl->GetWidth()) / img->getWidth();
	SetTransform(node, MEngine.environmentCtrl->GetWidth()/2, MEngine.environmentCtrl->GetHeight()/2, scale);
}

Vectorf3 MGameFactory::GetPercentageVec(float xPerc, float yPerc){
	float width = MEngine.environmentCtrl->GetWidth();
	float height = MEngine.environmentCtrl->GetHeight();

	return Vectorf3(width*xPerc, height*yPerc);
}



GNode* MGameFactory::CreateRoot(){

	GNode* _root = new GNode(ObjType::ROOT, 0, "root");


	//GNode* first = CreateFirst();
//	GNode* second = CreateSecond();
	GNode* third = CreateThird();


	//second->GetTransform().LocalPos.x = MEngine.environmentCtrl->GetWidth();
	//third->GetTransform().LocalPos.x = MEngine.environmentCtrl->GetWidth() * 2;

	_root->AddChild(third);
	//_root->AddChild(third);


//	_root->AddChild(first);
	

	return _root;

}

void MGameFactory::SwitchToScene(int scene){
	// nothing to do here...
}