#pragma once

#include "Component.h"
#include "ofxSQLite.h"

namespace Cog {

	/**
	* Wrapper for SQLite database
	*/
	class CogDatabase : public Component {
	private:
		// pointer to sqlite
		ofxSQLite* sql;
		// path to database
		string dbPath;

	public:

		~CogDatabase() {
			delete sql;
		}

		void OnInit() {

		}

		/**
		* Gets pointer to sqlite
		*/
		ofxSQLite* GetSQLite() {
			return sql;
		}

		/**
		* Initializes database from file
		* @param dbPath path to database file; if it doesn't exist, 
		* an empty database will be created
		*/
		void SetupDatabase(string dbPath) {
			this->dbPath = dbPath;
			sql = new ofxSQLite();
			sql->setup(dbPath.c_str());
		}

		/**
		* Closes SQLite database
		*/
		void CloseDatabase() {
			sqlite3_close(sql->getSQLitePtr());
		}

		/**
		* Removes database file
		*/
		void RemoveDBFile() {
			ofFile file;
			file.removeFile(dbPath.c_str());
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}
	};

}// namespace