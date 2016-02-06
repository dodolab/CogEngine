#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "ofxSQLite.h"
#include "SQLiteEntity.h"

#ifdef TESTING


#include "catch.hpp"
using namespace Cog;

class ofxSQLTestEntity : public ofxSQLiteEntity {
protected:

	void InternalCreate(ofxSQLiteInsert& insert) {
		insert
			.use("num", numVal)
			.use("str", strVal)
			.use("boolv", boolVal);
	}

	void InternalUpdate(ofxSQLiteUpdate& update) {
		update
			.use("num", numVal)
			.use("str", strVal)
			.use("boolv", boolVal);
	}

	void InternalLoad(ofxSQLiteSelect& select) {
		numVal = select.getInt("num");
		strVal = select.getString("str");
		boolVal = select.getBool("boolv");
	}

	string GetTableName() {
		return "TEST";
	}

public:

	int numVal;
	string strVal;
	bool boolVal;

	ofxSQLTestEntity() {

	}

	ofxSQLTestEntity(ofxSQLite* sqlite) :ofxSQLiteEntity(sqlite) {
		numVal = 0;
		strVal = "";
		boolVal = false;
	}

	static vector<ofxSQLTestEntity> GetList(ofxSQLite* sqlite) {
		ofxSQLTestEntity entdm;
		
		vector<ofxSQLTestEntity> output = vector<ofxSQLTestEntity>();
		ofxSQLiteSelect sel = sqlite->select("*").from(entdm.GetTableName().c_str());
		sel.execute().begin();

		while (sel.hasNext()) {
			ofxSQLTestEntity ent(sqlite);
			ent.InternalLoad(sel);
			output.push_back(ent);
			sel.next();
		}

		return output;
	}

	static void RemoveTable(ofxSQLite* sqlite) {
		sqlite->remove("TEST").execute();
	}

	static void CreateTableIfNotExists(ofxSQLite* sqlite) {
		sqlite->createTableIfNotExists("TEST")
			.add("id", SQLColumnType::SQLITE_COLUMN_INTEGER, true, true)
			.add("num", SQLColumnType::SQLITE_COLUMN_INTEGER)
			.add("str", SQLColumnType::SQLITE_COLUMN_TEXT, "")
			.add("boolv", SQLColumnType::SQLITE_COLUMN_BOOLEAN).execute();
	}
};

TEST_CASE("SQL test", "[class]")
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

		for (int i = 0; i < 100; i++) {
			ofxSQLTestEntity entity(sql);
			entity.boolVal = true;
			entity.numVal = i;;
			entity.strVal = "hello";
			entity.Save();
		}

		vector<ofxSQLTestEntity> list = ofxSQLTestEntity::GetList(sql);
		REQUIRE(list.size() == 100);
	}

	ofxSQLTestEntity::RemoveTable(sql);
	sqlite3_close(sql->getSQLitePtr());
	ofFile file;
	file.removeFile("test.db");
}

#endif
