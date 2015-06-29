#ifndef TARGET_WINDOWS

#include "ofMain.h"
#include "Enums.h"
#include "ofxSpriteSheetRenderer.h"
#include "ofxAndroid.h"


class ofAndroidApp : public ofxAndroidApp{
	uint64 absolute;
	uint64 delta;
	ofMesh mesh;		//Mesh
	ofLight light;		//Light
	void addRandomCircle( ofMesh &mesh ); //Main function which
	//moves circle and adds triangles to the object

	public:
	void setupViewports();
		void draw();
		void update();
void setup();

		void keyPressed  (int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();
};

//Universal function which sets normals
//for the triangle mesh
void setNormals( ofMesh &mesh );


#endif
