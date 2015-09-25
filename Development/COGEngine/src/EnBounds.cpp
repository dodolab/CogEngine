#include "EnBounds.h"

bool EnBounds::Collides(GNode& first, GNode& second) const{

	// image collision
	spt<ofImage> img1 = first.GetShape<spt<EnImageShape>>()->GetImage();
	spt<ofImage> img2 = second.GetShape<spt<EnImageShape>>()->GetImage();

	ofVec3f pos1 = first.GetTransform().absPos;
	ofVec3f pos2 = second.GetTransform().absPos;

	ofVec3f scale1 = first.GetTransform().absScale;
	ofVec3f scale2 = second.GetTransform().absScale;

	float width1 = abs(img1->width*scale1.x);
	float width2 = abs(img2->width*scale2.x);

	float height1 = abs(img1->height*scale1.y);
	float height2 = abs(img2->height*scale2.y);

	// check collision
	bool collidesInX = (max(pos1.x + width1 / 2, pos2.x + width2 / 2) - min(pos1.x - width1 / 2, pos2.x - width2 / 2)) <= (width1 + width2);
	bool collidesInY = (max(pos1.y + height1 / 2, pos2.y + height2 / 2) - min(pos1.y - height1 / 2, pos2.y - height2 / 2)) <= (height1 + height2);

	return collidesInX && collidesInY;
}

