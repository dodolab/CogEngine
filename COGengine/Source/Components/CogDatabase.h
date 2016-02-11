#pragma once

#include "Component.h"
#include "ofxSQLite.h"

namespace Cog {

	/**
	* Database manager
	*/
	class CogDatabase : public Component{
	
		OBJECT(CogDatabase)
	
	private:
		ofxSQLite* sql;
		string dbPath;

	public:

		~CogDatabase() {
			delete sql;
		}

		void Init() {

		}

		void Init(spt<ofxXml> xml) {

		}

		ofxSQLite* GetSQLite() {
			return sql;
		}

		void SetupDatabase(string dbPath) {
			this->dbPath = dbPath;
			sql = new ofxSQLite();
			sql->setup(dbPath.c_str());
		}

		void CloseDatabase() {
			sqlite3_close(sql->getSQLitePtr());
		}

		void RemoveDBFile() {
			ofFile file;
			file.removeFile(dbPath.c_str());
		}
	};

}// namespace