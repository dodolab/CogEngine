#pragma once
#include "ofxSQLite.h"

namespace Cog {

	/**
	* Base class for all SQL entities
	*/
	class SQLiteEntity {
	protected:
		// link to sqlite 
		ofxSQLite* sqlite;
		// identifier
		int id;

		virtual void InternalCreate(ofxSQLiteInsert& insert) = 0;
		virtual void InternalUpdate(ofxSQLiteUpdate& update) = 0;
		virtual void InternalLoad(ofxSQLiteSelect& select) = 0;
	
		virtual string GetIdName() {
			return "id";
		}

		virtual string GetTableName() = 0;


	public:
		SQLiteEntity() {

		}

		SQLiteEntity(ofxSQLite* sqlite) :sqlite(sqlite) {
			this->id = 0;
		}

		/**
		* Gets entity id
		*/
		int GetId() {
			return this->id;
		}

		/**
		* Loads entity by id
		*/
		void LoadById(int id) {
			ofxSQLiteSelect select = sqlite->select("*")
				.from(GetTableName().c_str())
				.where(GetIdName().c_str(), id)
				.execute().begin();
			this->id = select.getInt(GetIdName());
			this->InternalLoad(select);
		}

		/**
		* Creates or updates the entity
		*/
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

		/**
		* Removes the entity
		*/
		int Remove() {
			return sqlite->remove(GetTableName().c_str())
				.where(GetIdName().c_str(), this->id)
				.execute();
		}
	};

}// namespace