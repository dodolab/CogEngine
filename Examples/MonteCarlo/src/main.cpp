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
#include "UCTAgent.h"

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


#define CATCH_CONFIG_RUNNER

#include "MonteCarloTest.h"

int main() {
	
	ofSetupOpenGL(800, 450, OF_WINDOW);
	auto start = ofGetSystemTimeMicros();

	//ofSeedRandom();
	auto simulator = spt<HydroqSimulator>(new HydroqSimulator());
	auto agents = vector<spt<AIAgent<HydroqState, HydroqAction>>>();
	//agents.push_back(spt<RandomAgentSmazat<HydroqState, HydroqAction>>(new RandomAgentSmazat<HydroqState, HydroqAction>()));

	agents.push_back(spt<UCTAgent<HydroqState, HydroqAction>>(new UCTAgent<HydroqState, HydroqAction>("firstAgent", 20, sqrt(2))));
	agents.push_back(spt<UCTAgent<HydroqState, HydroqAction>>(new UCTAgent<HydroqState, HydroqAction>("firstAgent", 20, sqrt(2))));
	//agents.push_back(spt<RandomAgent<HydroqState, HydroqAction>>(new RandomAgent<HydroqState, HydroqAction>()));

	
	auto mt = new MonteCarloSearch<HydroqState, HydroqAction>(simulator,agents);
	mt->RunSearch(1);

	auto end = ofGetSystemTimeMicros();
	auto diff = (end - start) / 1000;
	

	ofRunApp(new ExampleApp());
	return 0;
}


