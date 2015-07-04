#include "EnBounds.h"

bool EnBounds::Collides(GNode& first, GNode& second) const{
	spt<ofImage> img1 = first.GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
	spt<ofImage> img2 = second.GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);

	ofVec3f pos1 = first.GetTransform().AbsPos;
	ofVec3f pos2 = second.GetTransform().AbsPos;

	ofVec3f scale1 = first.GetTransform().AbsScale;
	ofVec3f scale2 = second.GetTransform().AbsScale;

	float width1 = abs(img1->width*scale1.x);
	float width2 = abs(img2->width*scale2.x);

	float height1 = abs(img1->height*scale1.y);
	float height2 = abs(img2->height*scale2.y);

	// check collision
	bool collidesInX = (max(pos1.x + width1 / 2, pos2.x + width2 / 2) - min(pos1.x - width1 / 2, pos2.x - width2 / 2)) <= (width1 + width2);
	bool collidesInY = (max(pos1.y + height1 / 2, pos2.y + height2 / 2) - min(pos1.y - height1 / 2, pos2.y - height2 / 2)) <= (height1 + height2);

	return collidesInX && collidesInY;
}

