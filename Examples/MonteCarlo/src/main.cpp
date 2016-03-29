#include "ofxCogMain.h"
#include "AnimLoader.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "ofxNetwork.h"
#include "Vec2i.h"

enum class ActionType {
	TAKE_EMPTY_RIG, TAKE_ENEMY_RIG, WAIT
};

class Action {
public:
	ActionType type;

	void Print() {
		if (type == ActionType::TAKE_EMPTY_RIG) cout << "-- vezmu prazdny rig" << endl;
		else if (type == ActionType::TAKE_ENEMY_RIG) cout << "-- vezmu enemy rig" << endl;
		else cout << "-- cekam" << endl;
	}
};


class Plan {
public:
	vector<Action> actions;
	int actualAction = 0;

	bool IsCompleted() {
		return actualAction == (actions.size() - 1);
	}

	void Restart() {
		actualAction = 0;
	}

	Action& TakeAction() {
		return actions[actualAction++];
	}

	void Print() {
		cout << "Tisknu plan" << endl;
		for (auto act : actions) act.Print();
	}
};


class State {
public:
	int emptyRigs; // not used rigs
	int myRigs; // my rigs
	int enemyRigs; // rigs owned by enemy

	float Eval() {
		return myRigs * 10 - enemyRigs * 20;
	}
};

class MonteCarlo {
public:

	Plan EvalPlan(State currentState, int plansToEval, int simsPerEval, int maxStepsPerSim) {
		
		float bestVal = -100000;
		Plan best;
		
		for (int i = 0; i < plansToEval; i++) {
			Plan plan = GeneratePlan(currentState);
			float val = EvaluatePlan(plan, currentState, simsPerEval, maxStepsPerSim);
			if (val > bestVal) {
				best = plan;
				bestVal = val;
			}
		}

		return best;
	}

	Plan GeneratePlan(State state) {
		
		Plan pl = Plan();
		for (int i = 0; i < 5; i++) {
			Action act = Action();
			if (ofRandom(0, 1) < 0.5f) act.type = ActionType::TAKE_EMPTY_RIG;
			else if (ofRandom(0, 1) < 0.5f) act.type = ActionType::TAKE_ENEMY_RIG;
			else act.type = ActionType::WAIT;
			pl.actions.push_back(act);
		}

		return pl;
	}

	Plan GenerateOpponentPlan(State state) {
		Plan pl = Plan();
		for (int i = 0; i < 5; i++) {
			Action act = Action();
			act.type = ActionType::WAIT;
			pl.actions.push_back(act);
		}

		return pl;
	}

	float EvaluatePlan(Plan plan, State state, int simulations, int maxSteps) {
		float min = 100000;

		for (int i = 0; i < simulations; i++) {
			float val = SimulatePlan(plan, state, maxSteps);
			if (val < min) min = val;
		}
		return min;
	}

	float SimulatePlan(Plan& plan, State state, int maxSteps) {
		Plan& aiPlan = plan;

		Plan opponentPlan = GenerateOpponentPlan(state);

		while (true) {
			SimulatePlanStep(aiPlan, opponentPlan, state);

			if (--maxSteps <= 0) break;

			if (aiPlan.IsCompleted()) {
				aiPlan.Restart();
				break;
			}

			if (opponentPlan.IsCompleted()) {
				opponentPlan = GenerateOpponentPlan(state);
			}
		}
		return state.Eval();
	}

	void SimulatePlanStep(Plan& aiPlan, Plan& opponentPlan, State& aiState) {
		Action& aiAction = aiPlan.TakeAction();
		Action& oppAction = opponentPlan.TakeAction();
	
		if (aiAction.type == ActionType::TAKE_EMPTY_RIG && aiState.emptyRigs > 0) {
			aiState.emptyRigs--;
			aiState.myRigs++;
		}
		else if (aiAction.type == ActionType::TAKE_ENEMY_RIG && aiState.enemyRigs > 0) {
			aiState.enemyRigs--;
			aiState.myRigs++;
		}
		else aiAction.type = ActionType::WAIT;
		
		if (oppAction.type == ActionType::TAKE_EMPTY_RIG && aiState.emptyRigs > 0) {
			aiState.emptyRigs--;
			aiState.enemyRigs++;
		}
		else if (oppAction.type == ActionType::TAKE_ENEMY_RIG && aiState.myRigs > 0) {
			aiState.enemyRigs++;
			aiState.myRigs--;
		}
	}
};



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

	MonteCarlo mc = MonteCarlo();
	State st = State();
	st.emptyRigs = 20;
	st.enemyRigs = 0;
	st.myRigs = 0;

	Plan myPlan = mc.EvalPlan(st, 100, 100, 10);
	myPlan.Print();

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


