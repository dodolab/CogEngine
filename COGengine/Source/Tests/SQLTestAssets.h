#pragma once

#include "ofxSQLite.h"
#include "SQLiteEntity.h"


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

		vector<ofxSQLTestEntity> output;
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
