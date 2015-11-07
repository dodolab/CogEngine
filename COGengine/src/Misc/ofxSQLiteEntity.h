#pragma once
#include "ofxSQLite.h"

#define SQL_BOOL "BOOLEAN"
#define SQL_DATETIME "DATETIME"
#define SQL_CREATETABLE "CREATE TABLE IF NOT EXISTS"
#define SQL_PKAUTOINCREMENT "INTEGER PRIMARY KEY AUTOINCREMENT"
#define SQL_VARCHAR(size) "VARCHAR("+size+")"
#define SQL_DEFAULT "DEFAULT"
#define SQL_BEGINTRANS "BEGIN TRANSACTION"
#define SQL_COMMIT "COMMIT;"

class ofxSQLiteEntity{
protected:
	ofxSQLite* sqlite;

	int id;

	virtual void InternalCreate(ofxSQLiteInsert& insert) = 0;
	virtual void InternalUpdate(ofxSQLiteUpdate& update) = 0;
	virtual void InternalLoad(ofxSQLiteSelect& select) = 0;
	virtual string GetIdName(){
		return "id";
	}

	virtual string GetTableName() = 0;


public:

	ofxSQLiteEntity(ofxSQLite* sqlite) :sqlite(sqlite){
		this->id = 0;
	}

	int GetId(){
		return this->id;
	}

	void GetById(int id){
		ofxSQLiteSelect select = sqlite->select("*")
			.from(GetTableName().c_str())
			.where(GetIdName().c_str(), id)
			.execute().begin();
		this->id = select.getInt(GetIdName());
		this->InternalLoad(select);
	}

	int Save(){
		if (this->id == 0){
			// save
			ofxSQLiteInsert insert = sqlite->insert(GetTableName().c_str());
			this->InternalCreate(insert);
			int output = insert.execute();
			this->id = sqlite->lastInsertID();
			return output;
		}
		else{
			ofxSQLiteUpdate update = sqlite->update(GetTableName().c_str());
			this->InternalUpdate(update);
			return update.where(GetIdName().c_str(), this->id)
				.execute();
		}
	}

	int Remove(){
		return sqlite->remove(GetTableName().c_str())
			.where(GetIdName().c_str(), this->id)
			.execute();
	}

};

class ofxSQLKrabicka : public ofxSQLiteEntity{
protected:

	void InternalCreate(ofxSQLiteInsert& insert){
		 insert
			 .use("tlacitko", tlacitko)
			 .use("neco", neco)
			 .use("jeSpatne", jeSpatne);
	}

	void InternalUpdate(ofxSQLiteUpdate& update){
		update
			.use("tlacitko", tlacitko)
			.use("neco", neco)
			.use("jeSpatne", jeSpatne);
	}

	void InternalLoad(ofxSQLiteSelect& select){
		tlacitko = select.getInt("tlacitko");
		neco = select.getString("neco");
		jeSpatne = select.getBool("jeSpatne");
	}

	string GetTableName(){
		return "Krabicka";
	}

public:

	int tlacitko;
	string neco;
	bool jeSpatne;


	ofxSQLKrabicka(ofxSQLite* sqlite) :ofxSQLiteEntity(sqlite){
		tlacitko = 0;
		neco = "";
		jeSpatne = false;
	}

	virtual vector<ofxSQLKrabicka> GetList(){
		vector<ofxSQLKrabicka> output = vector<ofxSQLKrabicka>();
		ofxSQLiteSelect sel = sqlite->select("*").from(GetTableName().c_str());
		sel.execute().begin();

		while (sel.hasNext()) {
			ofxSQLKrabicka krab(sqlite);
			krab.InternalLoad(sel);
			output.push_back(krab);
			sel.next();
		}

		return output;
	}
};