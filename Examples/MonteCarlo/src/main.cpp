#include "ofxCogMain.h"
#include "AnimLoader.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "ofxNetwork.h"
#include "Vec2i.h"
#include "MonteCarloSearch.h"
#include "HydroqSimulator.h"
#include "HydroqAction.h"
#include "RandomAgent.h"

class ExampleApp : public CogApp {
public:

	ExampleApp() {
		
	}

	void InitComponents() {

	}

	void InitEngine() {
		

	}

	void InitStage(Stage* stage) {
	}
};



int main() {

	auto simulator = spt<HydroqSimulator>(new HydroqSimulator());
	auto agents = vector<spt<AIAgent<HydroqState,HydroqAction>>>();
	agents.push_back(spt<RandomAgent<HydroqState, HydroqAction>>(new RandomAgent<HydroqState,HydroqAction>()));
	agents.push_back(spt<RandomAgent<HydroqState, HydroqAction>>(new RandomAgent<HydroqState,HydroqAction>()));

	auto mt = new MonteCarloSearch<HydroqState, HydroqAction>(simulator,agents);
	mt->RunSearch(1);

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


