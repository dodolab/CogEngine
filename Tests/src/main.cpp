#define CATCH_CONFIG_RUNNER

// tests can run only in debug mode
#ifdef _DEBUG
#include "FlagsTest.h"
#include "TransformTest.h"
#include "SQLTest.h"
#include "MeasureTest.h"
#include "NetworkTest.h"
#include "StateMachineTest.h"
#include "GoalTest.h"
#include "SettingsTest.h"
#include "EngineTest.h"
#include "StrIdTest.h"
#include "LuaTest.h"
#include "MonteCarloTest.h"	
#include "CoroutineTest.h"
#include "MathTest.h"
#include "TimelineTest.h"

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	int result = Catch::Session().run();
	return result;
}

#endif