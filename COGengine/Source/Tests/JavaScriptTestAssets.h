#pragma once

#include "duktape.h"
#include "dukbind.h"


#define behavior_test "\
function TestingBehavior(parameter) {\
    this.updateCounter = 0;\
	this.parameter = parameter;\
}\
\
\
TestingBehavior.prototype.OnMessage = function(action, behaviorId) {\
\
	if (action == StrId('UPDATE_PARAM')) {\
		this.parameter++;\
	}\
};\
\
TestingBehavior.prototype.Update = function(delta, absolute) {\
	this.updateCounter++;\
};"


struct TestPointer
{
	TestPointer(const int data) : Data(data) {}
	TestPointer(const TestPointer & other)
	{
		Data = other.Data;
		++CopyConstructorCount;
	}

	~TestPointer()
	{
		++DestructorCount;
	}

	int Data;

	static size_t CopyConstructorCount;
	static size_t DestructorCount;
};

size_t TestPointer::CopyConstructorCount = 0;
size_t TestPointer::DestructorCount = 0;

dukbind_bind_as_raw_pointer(TestPointer)

static bool DoStuffIsCalled = false;

static duk_ret_t DoStuff(duk_context *)
{
	DoStuffIsCalled = true;
	return 0;
}

static duk_ret_t CheckThis(duk_context * ctx)
{
	duk_push_this(ctx);
	const TestPointer & result = dukbind::Get(ctx, -1, (TestPointer*)0);
	REQUIRE(result.Data == 5678);

	return 0;
}


int DuktapeGlobalTestAdd(duk_context *ctx) {
	// get first parameter of a function
	int argument1 = duk_to_number(ctx, 0);
	// get second parameter of a function
	int argument2 = duk_to_number(ctx, 1);
	// push result
	duk_push_int(ctx, argument1 + argument2);
	return 1;
}

int expectedResult = 0;
int DuktapeResultTest(duk_context *ctx) {
	// get parameter of a function
	int result = duk_to_number(ctx, 0);
	REQUIRE(result == expectedResult);
	return 1;
}

// transforms string to StrId
int StrIdEval(duk_context *ctx) {
	string param = duk_to_string(ctx, 0);
	unsigned val = StrId(param).GetValue();
	duk_push_int(ctx, val);
	return 1;
}

class JavaScriptBehavior : public Behavior {
	duk_context* ctx;

public:

	JavaScriptBehavior(duk_context* context, int param) {
		this->ctx = context;

		if (duk_peval_string(ctx, behavior_test) != 0) {
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		}
		duk_pop(ctx);  /* ignore result */


					   // create new object of type "TestingBehavior"
		duk_push_global_object(ctx);
		// push parameter to the constructor
		duk_get_global_string(ctx, "TestingBehavior");
		duk_push_int(ctx, param);

		if (duk_pnew(ctx, 1) != 0) {
			printf("%s\n", duk_safe_to_string(ctx, -1));
		}

		// save object as "JavaScriptBehavior"
		duk_put_prop_string(ctx, -2, "JavaScriptBehavior");
	}

	void OnMessage(Msg& msg) {

		// get object JavaScriptBehavior
		duk_get_prop_string(ctx, -1, "JavaScriptBehavior");
		// push method OnMessage that will be called
		duk_get_prop_string(ctx, -1, "OnMessage");
		duk_dup(ctx, -2);
		duk_push_number(ctx, msg.GetAction());
		duk_push_number(ctx, msg.GetSenderId());
		if (duk_pcall_method(ctx, 2) != 0) {
			printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}
		// pop function
		duk_pop(ctx);
		// pop instance
		duk_pop(ctx);
	}

	int GetParameter() {
		duk_get_prop_string(ctx, -1, "JavaScriptBehavior");
		duk_get_prop_string(ctx, -1, "parameter");
		int param = duk_get_number(ctx, -1);
		duk_pop(ctx); // pop attribute
		duk_pop(ctx); // pop instance
		return param;
	}

	int GetUpdateCounter() {
		duk_get_prop_string(ctx, -1, "JavaScriptBehavior");
		duk_get_prop_string(ctx, -1, "updateCounter");
		int param = duk_get_number(ctx, -1);
		duk_pop(ctx); // pop attribute
		duk_pop(ctx); // pop instance
		return param;
	}

	void Update(const uint64 delta, const uint64 absolute) {

		// get object JavaScriptBehavior
		duk_get_prop_string(ctx, -1, "JavaScriptBehavior");
		// push method Update that will be called
		duk_get_prop_string(ctx, -1, "Update");
		// push two arguments
		duk_dup(ctx, -2);
		duk_push_number(ctx, delta);
		duk_push_number(ctx, absolute);
		// call method Update
		if (duk_pcall_method(ctx, 2) != 0) {
			printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
		}

		// pop function
		duk_pop(ctx);
		// pop instance
		duk_pop(ctx);
	}
};
