#include "ofApp.h"
#include "MGameEngine.h"
#include "CopterFactory.h"

bool sortByZIndex(basicSprite * a, basicSprite * b) {
	return a->pos.z > b->pos.z;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(50);   
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	// initialize game engine
	MEngine.Init(new CopterFactory(), new MGameStorage());
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();

	/*ofSetFrameRate(50);
	spriteRenderer = new ofxSpriteSheetRenderer(1, 10000, 0, 256); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
	spriteRenderer->loadTexture("images/blue.png", 256, GL_LINEAR); // load the spriteSheetExample.png texture of size 256x256 into the sprite sheet. set it's scale mode to nearest since it's pixel art

	ofEnableAlphaBlending(); // turn on alpha blending. important!
	*/
}

//--------------------------------------------------------------
void ofApp::update(){
	delta = ofGetSystemTime() - absolute;
	absolute = ofGetSystemTime();
	MEngine.Update(delta, absolute);
	
	/*
	spriteRenderer->clearCounters(); // clear the sheet

	sort(sprites.begin(), sprites.end(), sortByZIndex); // sorts the sprites vertically so the ones that are lower are drawn later and there for in front of the ones that are higher up

	if (sprites.size() > 0) // if we have sprites
	{
		for (int i = sprites.size() - 1; i >= 0; i--) //go through them
		{
			sprites[i]->pos.y += sprites[i]->speed; //add their speed to their y
			sprites[i]->rotation+=0.1f;
			if (sprites[i]->pos.y > ofGetHeight() + 16) //if they are past the bottom of the screen
			{
				delete sprites[i]; //delete them
				sprites.erase(sprites.begin() + i); // remove them from the vector
			}
			else{ //otherwise 
			   spriteRenderer->addTile(sprites[i]->index, sprites[i]->frame, sprites[i]->pos.x, sprites[i]->pos.y, sprites[i]->pos.z, -1, sprites[i]->w, sprites[i]->h, F_NONE, 0.1f, sprites[i]->rotation); // add them to the sprite renderer (add their animation at their position, there are a lot more options for what could happen here, scale, tint, rotation, etc, but the animation, x and y are the only variables that are required)
		
			   //spriteRenderer->setBrushIndex(0, 1);
			   //spriteRenderer->addRect(sprites[i]->pos.x, sprites[i]->pos.y, sprites[i]->pos.z, 20, 20, 1, sprites[i]->rotation, 255, 0, 0, 255, 0);
			}
		}
	}

	for (int i = 0; i < 10; i++) //lets add ten sprites every frame and fill the screen with an army
	{
		basicSprite * newSprite = new basicSprite(); // create a new sprite
		newSprite->pos.set(ofRandom(0, ofGetWidth()), ofRandom(0,5)); //set its position
		newSprite->speed = ofRandom(1, 5); //set its speed
		newSprite->rotation = ofRandom(0, PI);
		newSprite->index = 0;
		newSprite->frame=0;
		newSprite->total_frames = 1;
		newSprite->w = 1;
		newSprite->h = 1;
		sprites.push_back(newSprite); //add our sprite to the vector
	}*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	MEngine.Draw(delta, absolute);
	//spriteRenderer->draw(); //draw the sprites!
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	MEngine.environmentCtrl->OnKeyAction(key,true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	MEngine.environmentCtrl->OnKeyAction(key,false);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	MEngine.environmentCtrl->OnSingleTouchMotion(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x,y,button,true);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x,y,button,false);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	MEngine.environmentCtrl->OnScreenSizeChanged(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
