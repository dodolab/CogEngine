#pragma once
#include "ofxSQLite.h"

namespace Cog {

#define SQL_BOOL "BOOLEAN"
#define SQL_DATETIME "DATETIME"
#define SQL_CREATETABLE "CREATE TABLE IF NOT EXISTS"
#define SQL_PKAUTOINCREMENT "INTEGER PRIMARY KEY AUTOINCREMENT"
#define SQL_VARCHAR(size) "VARCHAR("+size+")"
#define SQL_DEFAULT "DEFAULT"
#define SQL_BEGINTRANS "BEGIN TRANSACTION"
#define SQL_COMMIT "COMMIT;"

	class ofxSQLiteEntity {
	protected:
		ofxSQLite* sqlite;

		int id;

		virtual void InternalCreate(ofxSQLiteInsert& insert) = 0;
		virtual void InternalUpdate(ofxSQLiteUpdate& update) = 0;
		virtual void InternalLoad(ofxSQLiteSelect& select) = 0;
		virtual string GetIdName() {
			return "id";
		}

		virtual string GetTableName() = 0;


	public:
		ofxSQLiteEntity() {

		}

		ofxSQLiteEntity(ofxSQLite* sqlite) :sqlite(sqlite) {
			this->id = 0;
		}

		int GetId() {
			return this->id;
		}

		void GetById(int id) {
			ofxSQLiteSelect select = sqlite->select("*")
				.from(GetTableName().c_str())
				.where(GetIdName().c_str(), id)
				.execute().begin();
			this->id = select.getInt(GetIdName());
			this->InternalLoad(select);
		}

		int Save() {
			if (this->id == 0) {
				// save
				ofxSQLiteInsert insert = sqlite->insert(GetTableName().c_str());
				this->InternalCreate(insert);
				int output = insert.execute();
				this->id = sqlite->lastInsertID();
				return output;
			}
			else {
				ofxSQLiteUpdate update = sqlite->update(GetTableName().c_str());
				this->InternalUpdate(update);
				return update.where(GetIdName().c_str(), this->id)
					.execute();
			}
		}

		int Remove() {
			return sqlite->remove(GetTableName().c_str())
				.where(GetIdName().c_str(), this->id)
				.execute();
		}

	};

}// namespace