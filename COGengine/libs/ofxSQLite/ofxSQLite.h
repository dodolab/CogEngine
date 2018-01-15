/**
 * When compiling the sqlite.c file I needed to put:
 * SQLITE_ENABLE_UPDATE_DELETE_LIMIT=1
 * in the build options.
 *
 * Or you can use a build define like: SQLITE_DEBUG 0
 *
 */
#ifdef SQLITE_DB
#ifndef OFXSQLITEH
#define OFXSQLITEH
#define DSQLITE_ENABLE_UPDATE_DELETE_LIMIT 1


#include <vector>
#include <string>
#include "sqlite3.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "ofUtils.h"


class ofxSQLiteSimpler;

enum {
	OFX_SQLITE_TYPE_INT
	, OFX_SQLITE_TYPE_DOUBLE
	, OFX_SQLITE_TYPE_LONG
	, OFX_SQLITE_TYPE_INT64
	, OFX_SQLITE_TYPE_TEXT
	, OFX_SQLITE_TYPE_NULL
};

enum class SQLColumnType{
	SQLITE_COLUMN_INTEGER,
	SQLITE_COLUMN_VARCHAR,
	SQLITE_COLUMN_BOOLEAN,
	SQLITE_COLUMN_DATETIME,
	SQLITE_COLUMN_TEXT
};

class ofxSQLiteType {
public:
	virtual std::string getString()		{ return	""; }
	virtual int getInt()				{ return	0; }
	virtual long getLong()				{ return	0; }
	virtual uint64_t getUint64()		{ return  0; }
	virtual int getType() = 0;
};

class ofxSQLiteTypeNow : public ofxSQLiteType {
public:
	virtual std::string getString() {
		time_t now = time(0);
		std::stringstream strm;
		strm << now;
		return strm.str();
	}

	virtual int getType() {
		return OFX_SQLITE_TYPE_TEXT;
	}
};




class ofxSQLiteAbstract {
public:
	ofxSQLiteAbstract(sqlite3* pSQLite) :sqlite(pSQLite) {
	}

	sqlite3* sqlite;

private:

};

enum OperatorType {
	OP_GREATER_THAN
	, OP_GREATER_EQUAL_THAN
	, OP_LESS_THAN
	, OP_LESS_EQUAL_THAN
	, OP_LIKE
	, OP_EQUAL
};

struct FieldValuePair {
	std::string field;
	std::string value_string;
	int value_int;
	double value_double;
	long value_long;
	uint64_t value_uint64;
	int index;
	int type;
	int sql_operator;

	void bind(sqlite3_stmt* pStatement) {
		int result = SQLITE_OK;
		switch (type) {
		case OFX_SQLITE_TYPE_INT:	{
			result = sqlite3_bind_int(pStatement, index, value_int);
			break;
		}
		case OFX_SQLITE_TYPE_LONG: {
			result = sqlite3_bind_int64(pStatement, index, value_long);
			break;
		}
		case OFX_SQLITE_TYPE_INT64: {
			result = sqlite3_bind_int64(pStatement, index, value_uint64);
			break;
		}
		case OFX_SQLITE_TYPE_TEXT: {
			result = sqlite3_bind_text(
				pStatement
				, index
				, value_string.c_str()
				, value_string.size()
				, SQLITE_TRANSIENT
				);
			break;
		}
		case OFX_SQLITE_TYPE_DOUBLE: {
			result = sqlite3_bind_double(pStatement, index, value_double);
			break;
		}
		default:break;
		}

		if (result != SQLITE_OK) {
			printf("SQLITE: Error while binding parameter: %d\n", index);
		}
	}

	void setOperatorType(int op) {
		sql_operator = op;
	}

	std::string getFieldAndValueForQuery(bool embedValue = false) {
		std::string result = "";
		std::string value_part = "";

		// create value part.
		if (embedValue) {
			value_part = "\"" + valueString() + "\"";
		}
		else {
			value_part = " ?" + indexString() + " ";
		}

		// create field + value part.
		switch (sql_operator) {
		case OP_GREATER_THAN: {
			result = field + " > " + value_part;
			break;
		}
		case OP_GREATER_EQUAL_THAN: {
			result = field + " >= " + value_part;
			break;
		}
		case OP_LESS_THAN: {
			result = field + " < " + value_part;
			break;
		}
		case OP_LESS_EQUAL_THAN: {
			result = field + " <= " + value_part;
			break;
		}
		case OP_EQUAL:{
			result = field + " = " + value_part;
			break;
		}
		case OP_LIKE: {
			result = field + " LIKE " + value_part;
			break;
		}
		default:{
			printf("Unhandled operator type %d for field: %s.\n", sql_operator, field.c_str());
			break;
		}
		}
		return result;
	}

	std::string indexString() {
		std::stringstream ss;
		ss << index;
		return ss.str();
	}

	std::string valueString() {
		std::string result = "";
		switch (type) {
		case OFX_SQLITE_TYPE_INT: {
			std::stringstream ss;
			ss << value_int;
			result = ss.str();
			break;
		}
		case OFX_SQLITE_TYPE_LONG: {
			std::stringstream ss;
			ss << value_long;
			result = ss.str();
			break;
		}
		case OFX_SQLITE_TYPE_INT64: {
			std::stringstream ss;
			ss << value_uint64;
			result = ss.str();
			break;
		}
		case OFX_SQLITE_TYPE_TEXT: {
			result = value_string;
			break;
		}
		}
		return result;
	}
};

class ofxSQLiteFieldValues {
public:
	ofxSQLiteFieldValues();
	int use(std::string sField, int nValue);
	int use(std::string sField, unsigned long nValue);
	int use(std::string sField, uint64_t nValue);
	int use(std::string sField, float nValue);
	int use(std::string sField, long nValue);
	int use(std::string sField, double nValue);
	int use(std::string sField, std::string sValue);
	int use(std::string sField, ofxSQLiteType& oValue);
	int use(std::string sField);
	void bind(sqlite3_stmt* pStatement);

	FieldValuePair& at(int nIndex);
	void begin();
	FieldValuePair current();
	void next();
	bool hasNext();
	int size();

private:
	std::vector<FieldValuePair> field_values;
	int nextFieldIndex();
	int _index;
};


class ofxSQLiteInsert {
public:
	ofxSQLiteInsert(sqlite3* pSQLite, std::string sTable);
	std::string getLiteralQuery();
	ofxSQLiteFieldValues getFields();

	template<typename T>
	ofxSQLiteInsert& use(std::string sField, T sValue) {
		field_values.use(sField, sValue);
		return *this;
	}
	int execute();

private:
	std::string table;
	sqlite3* sqlite;
	int pair_count;
	ofxSQLiteFieldValues field_values;
};

using namespace std; // tmp


enum WhereTypes {
	WHERE
	, WHERE_AND
	, WHERE_OR
	, WHERE_LIKE
	, WHERE_OR_LIKE
	, WHERE_AND_LIKE
};

struct Where {
	int field_index;
	int type;

	std::string getAndOr(bool isFirstWhereClause = false) {
		std::string result;
		if (!isFirstWhereClause) {
			if (type == WHERE_AND) {
				result += " AND ";
			}
			else if (type == WHERE_OR) {
				result += " OR ";
			}
			else if (type == WHERE_OR_LIKE) {
				result += " OR  ";
			}
			else if (type == WHERE_AND_LIKE) {
				result += " AND  ";
			}
		}
		else {
			return " WHERE ";
		}
		return result;
	}
};

class ofxSQLiteWhere {
public:

	// where clause..
	template<typename T>
	ofxSQLiteWhere& where(std::string sField, T mValue) {
		return where(sField, mValue, WHERE);
	}

	template<typename T>
	ofxSQLiteWhere& orWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_OR);
	}

	template<typename T>
	ofxSQLiteWhere& andWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_AND);
	}

	template<typename T>
	ofxSQLiteWhere& where(std::string sField, T mValue, int nType) {
		// It's also possible to use the where like:
		// andWhere('fieldname > ?',value), by default we use "=" as
		// comparator.
		std::stringstream ss(sField);
		std::string part;
		std::string prev_part = "";
		int sql_operator = 0;
		while (ss) {
			ss >> part;
			if (part == "<") {
				sql_operator = OP_LESS_THAN;
			}
			else if (part == ">") {
				sql_operator = OP_GREATER_THAN;
			}
			else if (part == "<=") {
				sql_operator = OP_LESS_EQUAL_THAN;
			}
			else if (part == ">=") {
				sql_operator = OP_GREATER_EQUAL_THAN;
			}

			if (sql_operator != 0) {
				//div = part;
				//has_questionmark = true;
				sField = prev_part;
				break;
			}
			prev_part = part;
		}

		// when no other operator found we use the default one...
		if (sql_operator == 0) {
			sql_operator = OP_EQUAL;
		}
		if (nType == WHERE_LIKE || nType == WHERE_OR_LIKE || nType == WHERE_AND_LIKE) {
			sql_operator = OP_LIKE;
		}

		int field_value_index = where_values.use(sField, mValue);
		where_values.at(field_value_index).setOperatorType(sql_operator);

		struct Where where;
		where.type = nType;
		where.field_index = where_values.size() - 1;
		wheres.push_back(where);
		return *this;
	}

	ofxSQLiteWhere& whereNull(std::string sField) {
		where_values.use(sField);
		struct Where where;
		where.type = WHERE;
		where.field_index = where_values.size() - 1;
		wheres.push_back(where);
		return *this;
	}

	std::string getLiteralQuery(bool bFillValues = false) {
		std::string result = "";
		std::vector<Where>::iterator it = wheres.begin();
		int counter = 0;
		while (it != wheres.end()) {
			Where& where = *it;
			FieldValuePair value_pair = where_values.at(counter);

			if (value_pair.type == OFX_SQLITE_TYPE_NULL) {
				result += where.getAndOr(counter == 0) + value_pair.field + " is null ";
			}
			else {
				result += where.getAndOr(counter == 0);
				result += value_pair.getFieldAndValueForQuery();
			}
			++counter;
			++it;
		}
		return result;
	}

	void bind(sqlite3_stmt* pStatement) {
		where_values.bind(pStatement);
	}

	int size() {
		return where_values.size();
	}

private:
	ofxSQLiteFieldValues where_values;
	std::vector<Where> wheres;
};



class ofxSQLiteUpdate {
public:
	ofxSQLiteUpdate(sqlite3*  pSQLite, std::string sTable);

	template<typename T>
	ofxSQLiteUpdate& use(std::string sField, T sValue) {
		field_values.use(sField, sValue);
		return *this;
	}

	std::string getLiteralQuery();
	int execute();

	// where clause..
	template<typename T>
	ofxSQLiteUpdate& where(std::string sField, T mValue) {
		return where(sField, mValue, WHERE);
	}
	template<typename T>
	ofxSQLiteUpdate& orWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_OR);
	}
	template<typename T>
	ofxSQLiteUpdate& andWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_AND);
	}
	template<typename T>
	ofxSQLiteUpdate& where(std::string sField, T mValue, int nType) {
		wheres.where(sField, mValue, nType);
		return *this;
	}

private:
	std::string table;
	ofxSQLiteWhere wheres;
	ofxSQLiteFieldValues field_values;
	sqlite3* sqlite;
};





class ofxSQLiteDelete {
public:
	ofxSQLiteDelete(sqlite3*  pSQLite, std::string sTable);

	template<typename T>
	ofxSQLiteDelete& use(std::string sField, T sValue) {
		field_values.use(sField, sValue);
		return *this;
	}

	std::string getLiteralQuery(bool bFillValues = false);
	int execute();

	// where clause..
	template<typename T>
	ofxSQLiteDelete& where(std::string sField, T mValue) {
		return where(sField, mValue, WHERE);
	}
	template<typename T>
	ofxSQLiteDelete& orWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_OR);
	}
	template<typename T>
	ofxSQLiteDelete& andWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_AND);
	}
	template<typename T>
	ofxSQLiteDelete& where(std::string sField, T mValue, int nType) {
		wheres.where(sField, mValue, nType);
		return *this;
	}

private:
	std::string table;
	ofxSQLiteWhere wheres;
	ofxSQLiteFieldValues field_values;
	sqlite3* sqlite;
};

struct Join {
	std::string table;
	std::string condition;
	std::string fields;
};

struct Order {
	std::string field;
	std::string order;
};

class ofxSQLiteSelect {
public:
	ofxSQLiteSelect(sqlite3* pSQLite);

	// create select query
	ofxSQLiteSelect& select(std::string sFields);
	ofxSQLiteSelect& from(std::string sFrom);
	ofxSQLiteSelect& join(std::string sTable, std::string sOnField, std::string sFields);

	// where clause..
	template<typename T>
	ofxSQLiteSelect& where(std::string sField, T mValue) {
		return where(sField, mValue, WHERE);
	}

	template<typename T>
	ofxSQLiteSelect& orWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_OR);
	}

	template<typename T>
	ofxSQLiteSelect& orLike(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_OR_LIKE);
	}

	template<typename T>
	ofxSQLiteSelect& andWhere(std::string sField, T mValue) {
		return where(sField, mValue, WHERE_AND);
	}

	template<typename T>
	ofxSQLiteSelect& where(std::string sField, T mValue, int nType) {
		wheres.where(sField, mValue, nType);
		return *this;
	}

	ofxSQLiteSelect& whereNull(std::string sField) {
		wheres.whereNull(sField);
		return *this;
	}

	ofxSQLiteSelect& limit(int nCount, int nOffset);
	ofxSQLiteSelect& limit(int nCount);

	ofxSQLiteSelect& order(std::string sField);
	ofxSQLiteSelect& order(std::string sField, std::string sOrder);

	// execution
	ofxSQLiteSelect& execute();
	std::string getLiteralQuery(bool bFillValues = false);
	bool hasRow();

	// iterating
	ofxSQLiteSelect& begin();
	int next();
	bool hasNext();

	// fetching values.
	int getNumColumns();
	string getColumnName(int nColumNum);
	std::string getString(int nIndex = -1);
	int getInt(int nIndex = -1);
	float getFloat(int nIndex = -1);
	bool getBool(int nIndex = -1);

	std::string getString(string column);
	int getInt(string column);
	float getFloat(string column);
	bool getBool(string column);

	std::string getResultAsAsciiTable();
private:
	int getColumnIndex(string name);

	std::vector<Join> joins;
	ofxSQLiteWhere wheres;
	std::vector<Order> orders;

	ofxSQLiteFieldValues where_values;
	std::string fields;
	std::string from_table;
	sqlite3* sqlite;
	sqlite3_stmt* statement;
	int limit_offset;
	int limit_count;
	int col_index;
	int last_result;
};


struct ofxSQLiteColumn{

	ofxSQLiteColumn(){
		length = 0;
		primaryKey = false;
		autoIncrement = false;
		defaultVal = "";
	}

	string name;
	string defaultVal;
	SQLColumnType type;
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
	ofxSQLiteTable& add(std::string column, SQLColumnType type){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, SQLColumnType type, int length){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		col.length = length;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, SQLColumnType type, bool primaryKey, bool autoIncrement){
		ofxSQLiteColumn col = ofxSQLiteColumn();
		col.name = column;
		col.type = type;
		col.primaryKey = primaryKey;
		col.autoIncrement = autoIncrement;
		columns.push_back(col);
		return *this;
	}

	ofxSQLiteTable& add(std::string column, SQLColumnType type, string defaultVal){
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

		for (unsigned i = 0; i < columns.size(); i++){
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
	string GetColumnSpec(SQLColumnType type, int length){
		switch (type){
		case SQLColumnType::SQLITE_COLUMN_BOOLEAN:
			return "BOOLEAN";
			break;
		case SQLColumnType::SQLITE_COLUMN_DATETIME:
			return "DATETIME";
			break;
		case SQLColumnType::SQLITE_COLUMN_INTEGER:
			return "INTEGER";
			break;
		case SQLColumnType::SQLITE_COLUMN_TEXT:
			return "TEXT";
			break;
		case SQLColumnType::SQLITE_COLUMN_VARCHAR:
			return "VARCHAR(" + ofToString(length) + ")";
			break;
		}
		return "";
	}
};



class ofxSQLite {
	public:
		ofxSQLite();
		ofxSQLite(std::string sDB);
		void setup(std::string sDB);
		ofxSQLiteTable createTableIfNotExists(std::string sTable);
		ofxSQLiteInsert insert(std::string sTable);
		ofxSQLiteUpdate update(std::string sTable);
		ofxSQLiteDelete remove(std::string sTable);
		ofxSQLiteSelect select(std::string sFields);
		void printTable(std::string sTable);
		
		ofxSQLiteSimpler operator[](const std::string sKeyValue);
		
		int lastInsertID();
		const char* getError();

		int simpleQuery(const char* pSQL);
		inline ofxSQLiteTypeNow now() {
			return ofxSQLiteTypeNow();
		}
		
		inline sqlite3* getSQLitePtr(){
			return db;
		}

	private:
		sqlite3* db;
		std::string db_name;
		std::string db_file;
};


class ofxSQLiteSimpler {
public:
	ofxSQLiteSimpler(ofxSQLite& rDB, string sTable)
	:db(rDB)
	,table(sTable)
	{
	}
	
	template<typename T>
	ofxSQLiteSelect find(string sWhereField, T mWhereValue, string sSelectFields = "*") {
		ofxSQLiteSelect sel = db.select(sSelectFields).from(table).where(sWhereField, mWhereValue).execute();
		return sel;
	}
	
	// example: db["pakbox_users"].findOne("pu_id", 68, "pu_name").getString(0);
	template<typename T>
	ofxSQLiteSelect findOne(string sWhereField, T mWhereValue, string sSelectFields = "*") {
		ofxSQLiteSelect sel = db
			.select(sSelectFields)
			.from(table)
			.where(sWhereField, mWhereValue)
			.limit(1)
			.execute()
			.begin();
		
		return sel;
	}
	
	void print(){
		cout << db.select("*").from(table).execute().getResultAsAsciiTable();
	}
	
private:
	ofxSQLite& db;
	string table;

};

#endif
#endif