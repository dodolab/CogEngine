#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;

#include "JavaScriptTestAssets.h"



TEST_CASE("JavaScriptTest")
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