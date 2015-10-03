#pragma once

#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "ofxsqlite3.h"
#include "ofxSQLiteFieldValues.h"
#include "ofxSQLiteWhere.h"
#include "ofUtils.h"


using namespace std;

struct ofxSQLiteColumn{

	ofxSQLiteColumn(){
		length = 0;
		primaryKey = false;
		autoIncrement = false;
		defaultVal = "";
	}

	string name;
	string defaultVal;
	int type;
	int length;
	bool primaryKey;
	bool autoIncrement;
};

class ofxSQLiteTable {
private:
	string tableName;
	vector<ofxSQLiteColumn> columns;
	sqlite3* db;

public:
	ofxSQLiteTable(sqlite3* db, string name){
		this->db = db;
		this->tableName = name;
	}

	// add clause..
	ofxSQLiteTable& add(std::string column, int type){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, int type, int length){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		col.length = length;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, int type, bool primaryKey, bool autoIncrement){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		col.primaryKey = primaryKey;
		col.autoIncrement = autoIncrement;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, int type, string defaultVal){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		col.defaultVal = defaultVal;
		columns.push_back(col);
		return *this;
	}

	// execution
	int execute(){
		string literal = getLiteralQuery();
		const char* query = literal.c_str();

		sqlite3_stmt* statement;
		if (SQLITE_OK != sqlite3_prepare_v2(db, query, -1, &statement, 0)) {
			return sqlite3_errcode(db);
		}
		while (SQLITE_ROW == sqlite3_step(statement));
		return sqlite3_finalize(statement);

	}

	std::string getLiteralQuery(){
		std::stringstream ss;

		ss << "CREATE TABLE IF NOT EXISTS ";
		ss << tableName;
		ss << "( ";

		for (int i = 0; i < columns.size(); i++){
			ofxSQLiteColumn& col = columns[i];
			ss << col.name;
			ss << " ";
			ss << GetColumnSpec(col.type, col.length);
			ss << " ";

			if (col.primaryKey){
				ss << "PRIMARY KEY" << " ";
				if (col.autoIncrement){
					ss << "AUTOINCREMENT" << " ";
				}
			}

			if (col.defaultVal.length() != 0){
				ss << "DEFAULT" << " " << col.defaultVal.c_str() << " ";
			}

			if (i != columns.size() - 1) ss << ",";
		}


		ss << ");";
		return ss.str();
	}

private:
	string GetColumnSpec(int type, int length){
		switch (type){
		case SQLITE_COLUMN_BOOLEAN:
			return "BOOLEAN";
			break;
		case SQLITE_COLUMN_DATETIME:
			return "DATETIME";
			break;
		case SQLITE_COLUMN_INTEGER:
			return "INTEGER";
			break;
		case SQLITE_COLUMN_TEXT:
			return "TEXT";
			break;
		case SQLITE_COLUMN_VARCHAR:
			return "VARCHAR(" + ofToString(length) + ")";
			break;
		}
		return "";
	}
};

