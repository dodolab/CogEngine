#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;

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
		duk_push_number(ctx, msg.GetBehaviorId());
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


TEST_CASE("Class can be passed as pointer", "[binding][class]")
{
	duk_context * ctx = duk_create_heap_default();

	dukbind::BindingInfo info;

	info.AddClass("TestPointer", dukbind::rtti::GetTypeIndex<TestPointer>());
	info.AddMethod(dukbind::rtti::GetTypeIndex<TestPointer>(), "DoStuff", DoStuff);
	info.AddMethod(dukbind::rtti::GetTypeIndex<TestPointer>(), "CheckThis", CheckThis);

	dukbind::Setup(ctx, info, "Module");

	SECTION("Instance is not copied when pushed")
	{
		TestPointer data(1234);
		TestPointer::CopyConstructorCount = 0;
		dukbind::Push(ctx, data, (TestPointer*)0);
		REQUIRE(TestPointer::CopyConstructorCount == 0);
	}

	SECTION("Instance is not copied when pushed")
	{
		TestPointer data(5678);

		dukbind::Push(ctx, data, (TestPointer*)0);

		const TestPointer & result = dukbind::Get(ctx, -1, (TestPointer*)0);

		REQUIRE(&result == &data);
	}

	SECTION("Instance is not destructed when garbage collected")
	{
		TestPointer data(1234);
		dukbind::Push(ctx, data, (TestPointer*)0);
		TestPointer::DestructorCount = 0;

		duk_pop(ctx);
		duk_gc(ctx, 0);
		duk_gc(ctx, 0);
		REQUIRE(TestPointer::DestructorCount == 0);
	}

	SECTION("Binding is called")
	{
		TestPointer data(1234);
		duk_push_global_object(ctx);
		dukbind::Push(ctx, data, (TestPointer*)0);
		duk_put_prop_string(ctx, -2, "data");

		duk_eval_string_noresult(ctx, "data.DoStuff()");

		REQUIRE(DoStuffIsCalled);
	}

	SECTION("This is valid")
	{
		TestPointer data(5678);
		duk_push_global_object(ctx);
		dukbind::Push(ctx, data, (TestPointer*)0);
		duk_put_prop_string(ctx, -2, "data");

		duk_eval_string_noresult(ctx, "data.CheckThis()");
	}

	SECTION("Global object")
	{
		duk_push_global_object(ctx);
		// bind two global functions
		duk_push_c_function(ctx, DuktapeGlobalTestAdd, DUK_VARARGS);
		duk_put_prop_string(ctx, -2 /*idx:global*/, "DuktapeGlobalTestAdd");
		duk_push_c_function(ctx, DuktapeResultTest, DUK_VARARGS);
		duk_put_prop_string(ctx, -2 /*idx:global*/, "DuktapeResultTest");
		duk_pop(ctx);

		int output = 0;
		expectedResult = 25;
		output = duk_peval_string(ctx, "var arg1=12; var arg2=13; var result=DuktapeGlobalTestAdd(arg1, arg2); DuktapeResultTest(result);");
	}

	SECTION("Behavior test")
	{
		duk_push_global_object(ctx);
		// bind two global functions
		duk_push_c_function(ctx, StrIdEval, DUK_VARARGS);
		duk_put_prop_string(ctx, -2 /*idx:global*/, "StrId");
		duk_pop(ctx);

		JavaScriptBehavior* beh = new JavaScriptBehavior(ctx,12345);
		beh->Update(10, 10);
		beh->OnMessage(Msg(HandlingType(Scope::DIRECT_NO_TRAVERSE, false, false), StrId("UPDATE_PARAM"), 0, 0, nullptr, nullptr));

		int param = beh->GetParameter();
		int updateCnt = beh->GetUpdateCounter();

		REQUIRE(param == 12346);
		REQUIRE(updateCnt == 1);
	}

	duk_destroy_heap(ctx);
}