#pragma once

#include "ofApp.h"


bool sortByZIndex(basicSprite * a, basicSprite * b) {
	return a->pos.z > b->pos.z;
}


void MTestApp::setup(){
	ofSetFrameRate(50);
	spriteRenderer = new ofxSpriteSheetRenderer(1, 10000, 0, 256,256); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
	//spriteRenderer->allocate(512, GL_LINEAR);
	CollageTexture* text = new CollageTexture();
	text->allocate(512, 256, GL_RGBA, GL_LINEAR);
	text->pasteImage(0, 0, "images/blue.png", GL_RGBA);
	text->pasteImage(256, 0, "images/red.png", GL_RGBA);
	text->finish();
	ofImage img;
	ofPixels pixels;

	text->readToPixels(pixels);
	img.setFromPixels(pixels);
	img.saveImage("ffs.png");
	//spriteRenderer->loadTexture("images/blue.png", 512, GL_LINEAR);
	spriteRenderer->loadTexture(text); // load the spriteSheetExample.png texture of size 256x256 into the sprite sheet. set it's scale mode to nearest since it's pixel art

	ofEnableAlphaBlending(); // turn on alpha blending. important!
}

void MTestApp::draw(){
	spriteRenderer->draw(); //draw the sprites!
}

void MTestApp::update(){
	spriteRenderer->clearCounters(); // clear the sheet

	sort(sprites.begin(), sprites.end(), sortByZIndex); // sorts the sprites vertically so the ones that are lower are drawn later and there for in front of the ones that are higher up

	if (sprites.size() > 0) // if we have sprites
	{
		for (int i = sprites.size() - 1; i >= 0; i--) //go through them
		{
			sprites[i]->pos.y += sprites[i]->speed; //add their speed to their y
			sprites[i]->rotation += 0.1f;
			if (sprites[i]->pos.y > ofGetHeight() + 16) //if they are past the bottom of the screen
			{
				delete sprites[i]; //delete them
				sprites.erase(sprites.begin() + i); // remove them from the vector
			}
			else{ //otherwise 
				// add them to the sprite renderer (add their animation at their position, there are a lot more options for what could happen here, scale, tint, rotation, etc, but the animation, x and y are the only variables that are required)
				spriteRenderer->addTile(sprites[i]->index, sprites[i]->frame, sprites[i]->pos.x, sprites[i]->pos.y, sprites[i]->pos.z, -1, sprites[i]->w, sprites[i]->h, F_NONE, 1.0f, sprites[i]->rotation);

				//spriteRenderer->setBrushIndex(0, 1);
				//spriteRenderer->addRect(sprites[i]->pos.x, sprites[i]->pos.y, sprites[i]->pos.z, 20, 20, 1, sprites[i]->rotation, 255, 0, 0, 255, 0);
			}
		}
	}

	for (int i = 0; i < 10; i++) //lets add ten sprites every frame and fill the screen with an army
	{
		basicSprite * newSprite = new basicSprite(); // create a new sprite
		newSprite->pos.set(ofRandom(0, ofGetWidth()), ofRandom(0, 5)); //set its position
		newSprite->speed = ofRandom(1, 5); //set its speed
		newSprite->rotation = ofRandom(0, PI);
		newSprite->index = 0;
		newSprite->frame = 0;
		newSprite->total_frames = 1;
		newSprite->w = 1;
		newSprite->h = 1;
		sprites.push_back(newSprite); //add our sprite to the vector
	}
}

void MTestApp::keyPressed(int key){

}

void MTestApp::keyReleased(int key){

}

void MTestApp::windowResized(int w, int h){

}



#ifdef TARGET_WINDOWS

void MTestApp::mouseMoved(int x, int y){

}

void MTestApp::mouseDragged(int x, int y, int button){

}

void MTestApp::mousePressed(int x, int y, int button){

}

void MTestApp::mouseReleased(int x, int y, int button){

}

void MTestApp::dragEvent(ofDragInfo dragInfo){

}


#else


void MTestApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

void MTestApp::pause(){

}

void MTestApp::stop(){

}

void MTestApp::resume(){

}

void MTestApp::reloadTextures(){

}

bool MTestApp::backPressed(){
	return false;
}

void MTestApp::okPressed(){

}

void MTestApp::cancelPressed(){

}

void MTestApp::touchDown(int x, int y, int id){

}

void MTestApp::touchMoved(int x, int y, int id){

}

void MTestApp::touchUp(int x, int y, int id){

}

void MTestApp::touchDoubleTap(int x, int y, int id){

}

void MTestApp::touchCancelled(int x, int y, int id){

}

#endif
