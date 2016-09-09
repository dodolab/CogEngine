#include "MGameCtrl.h"
#include "SmartPointer.h"
#include "Iw2D.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "EnHitImage.h"
#include "BeHitEvent.h"

void MGameCtrl::Init(){

	_root = new GNode(ObjType::ROOT, 0, "root");

	// Load images
	spt<CIw2DImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");
	spt<CIw2DImage> rytmus = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");

	spt<CIwImage> imageB = MEngine.resourceCtrl->GetImage("images/blue.png");
	spt<CIwImage> imageRytmus = MEngine.resourceCtrl->GetImage("images/rytmus.png");

	spt<EnHitImage> hitImage = spt<EnHitImage>(new EnHitImage(imageB));
	spt<EnHitImage> hitRytmus = spt<EnHitImage>(new EnHitImage(imageRytmus));

	int pixels = MEngine.environmentCtrl->GetWidth();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* actual = _root;
	CIwColour color;
	CIwFVec2 actualSize = CIwFVec2(300, 300);

	for (int i = 0; i < 1; i++){
		color.Set(10+i*4, 100+i*20, 0);
		GNode* child = new GNode(ObjType::OBJECT, 0, "fofka");
		child->GetTransform().LocalPos = actualSize;
		if (i == 0){
			child->GetTransform().LocalPos = CIwFVec2(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2);
			child->GetTransform().Scale = scale * 30;
			child->AddAttr(Attrs::HIT_IMAGE, hitRytmus);
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

		color.Set(100+i*20, 10+i*4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = CIwFVec2(actualSize.x/2, actualSize.y/2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize/4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::HIT_IMAGE, hitImage);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = 0.4f;
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = CIwFVec2(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::HIT_IMAGE, hitImage);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = 0.4f;
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = CIwFVec2(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::HIT_IMAGE, hitImage);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = 0.4f;
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = CIwFVec2(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::HIT_IMAGE, hitImage);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = 0.4f;
		child->AddChild(sChild);

		actual = child;
		actualSize = actualSize / 2;
	}

	/*
	for (int i = 0; i < 1; i++){
		GNode* child = new GNode(ObjType::OBJECT, 0, "other");

		CIwFVec2 randomTransform(IwRandMinMax(1, MEngine.environmentCtrl->GetWidth()), IwRandMinMax(1, MEngine.environmentCtrl->GetHeight()));
		CIwFVec2 randomTransform2(IwRandMinMax(1, MEngine.environmentCtrl->GetWidth()), IwRandMinMax(1, MEngine.environmentCtrl->GetHeight()));

		child->GetTransform().LocalPos = CIwFVec2(0,0);
		//child->GetTransform().Scale = scale;
		//child->GetTransform().RotationOrigin = CIwFVec2(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2) - child->GetTransform().LocalPos;

		//child->AddAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE, image);

		CIwColour color;
		color.Set(125, 0, 0);

		child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, CIwFVec2(50,50));
		child->AddBehavior(new BeRender(RenderType::ARC));
		float speed = child->GetTransform().RotationOrigin.GetLength() / 600;

		BeRotateAnim* rot = new BeRotateAnim(0, 0, speed, false);
		//child->AddBehavior(rot);
		//child->AddBehavior(new BeRotateAnim(0, 0, speed, false));
		//child->AddBehavior(new BeTranslateAnim(randomTransform, randomTransform2, speed, true, true));

	    _root->AddChild(child);
		
	}
	

	GNode* child = new GNode(ObjType::OBJECT, 0, "rytmus");
	child->GetTransform().LocalPos = CIwFVec2(MEngine.environmentCtrl->GetWidth() / 2, MEngine.environmentCtrl->GetHeight() / 2);
	child->GetTransform().Scale = scale * 8;
	child->AddAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE, rytmus);
	child->AddBehavior(new BeRender(RenderType::IMAGE));
	child->AddBehavior(new BeRotateAnim(0, 0, 2, false));
	_root->AddChild(child);*/
}