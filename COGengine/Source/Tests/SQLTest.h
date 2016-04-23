#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"

#include "catch.hpp"
using namespace Cog;

#include "SQLTestAssets.h"










TEST_CASE("SQL test")
{
	ofxSQLite* sql = new ofxSQLite();
	sql->setup("test.db");

	SECTION("Simple SQL TEST")
	{
		ofxSQLTestEntity::RemoveTable(sql);
		ofxSQLTestEntity::CreateTableIfNotExists(sql);

		ofxSQLTestEntity entity(sql);
		entity.boolVal = true;
		entity.numVal = 12345;
		entity.strVal = "hello";
		entity.Save();

		vector<ofxSQLTestEntity> list = ofxSQLTestEntity::GetList(sql);
		auto first = list[0];

		REQUIRE(first.boolVal);
		REQUIRE(first.numVal == 12345);
		REQUIRE(first.strVal.compare("hello") == 0);
	}

	SECTION("Multiple SQL TEST")
	{
		ofxSQLTestEntity::RemoveTable(sql);
		ofxSQLTestEntity::CreateTableIfNotExists(sql);

		for (int i = 0; i < 10; i++) {
			ofxSQLTestEntity entity(sql);
			entity.boolVal = true;
			entity.numVal = i;;
			entity.strVal = "hello";
			entity.Save();
		}

		vector<ofxSQLTestEntity> list = ofxSQLTestEntity::GetList(sql);
		REQUIRE(list.size() == 10);
	}

	ofxSQLTestEntity::RemoveTable(sql);
	sqlite3_close(sql->getSQLitePtr());
	ofFile file;
	file.removeFile("test.db");
	delete sql;
}
