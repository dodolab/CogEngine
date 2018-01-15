#ifdef SQLITE_DB
#include "ofxSQLite.h"
#include "ofUtils.h"


// ====================== ofxSQLiteFieldValues ==============================

ofxSQLiteFieldValues::ofxSQLiteFieldValues() :_index(0) {
}

int ofxSQLiteFieldValues::use(std::string sField, int nValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_INT;
	field.value_int = nValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField, uint64_t nValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_INT64;
	field.value_uint64 = nValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField, long nValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_LONG;
	field.value_long = nValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField, unsigned long nValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_INT64;
	field.value_long = nValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField, std::string sValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_TEXT;
	field.value_string = sValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField, ofxSQLiteType& oValue) {
	switch (oValue.getType()) {
	case OFX_SQLITE_TYPE_INT64: return use(sField, oValue.getUint64()); break;
	case OFX_SQLITE_TYPE_LONG: return use(sField, oValue.getLong()); break;
	case OFX_SQLITE_TYPE_INT: return use(sField, oValue.getInt()); break;
	case OFX_SQLITE_TYPE_TEXT: return use(sField, oValue.getString()); break;
	default: break;
	};
	return 0;
}


int ofxSQLiteFieldValues::use(std::string sField, double nValue) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_DOUBLE;
	field.value_double = nValue;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}

int ofxSQLiteFieldValues::use(std::string sField) {
	FieldValuePair field;
	field.field = sField;
	field.type = OFX_SQLITE_TYPE_NULL;
	field.index = nextFieldIndex();
	field_values.push_back(field);
	return field_values.size() - 1;
}


int ofxSQLiteFieldValues::use(std::string sField, float nValue) {
	return use(sField, (double)nValue);

}

void ofxSQLiteFieldValues::begin() {
	_index = 0;
}

bool ofxSQLiteFieldValues::hasNext() {
	return (_index < (int)field_values.size());
}

void ofxSQLiteFieldValues::next() {
	if (hasNext()) _index++;
}

FieldValuePair ofxSQLiteFieldValues::current() {
	return field_values[_index];
}

FieldValuePair& ofxSQLiteFieldValues::at(int nIndex) {
	return field_values[nIndex];
}

int ofxSQLiteFieldValues::size() {
	return field_values.size();
}

void ofxSQLiteFieldValues::bind(sqlite3_stmt* pStatement) {
	begin();
	while (hasNext()) {
		current().bind(pStatement);
		next();
	}
}

int ofxSQLiteFieldValues::nextFieldIndex() {
	static int field_count = 1;
	field_count++;
	if (field_count >= 999) // you can only insert 999 columns
		field_count = 1;
	return field_count;
}


// ====================== ofxSQLiteInsert ==============================

ofxSQLiteInsert::ofxSQLiteInsert(sqlite3* pSQLite, std::string sTable) :sqlite(pSQLite), table(sTable), pair_count(0) {
}

ofxSQLiteFieldValues ofxSQLiteInsert::getFields() {
	return field_values;
}

std::string ofxSQLiteInsert::getLiteralQuery() {
	std::string sql = "INSERT INTO " + table + " (";
	std::string values = ") VALUES (";
	field_values.begin();
	while (field_values.hasNext()) {
		FieldValuePair field = field_values.current();
		field_values.next();

		sql += field.field + (field_values.hasNext() ? "," : "");
		values += "?" + (std::string)field.indexString() + (field_values.hasNext() ? "," : "");
	}
	return sql + values + ")";
}

int ofxSQLiteInsert::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(), -1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	// bind values.
	field_values.bind(statement);

	// execute the query.
	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	// cleanup.
	sqlite3_finalize(statement);
	return SQLITE_OK;
}


// ====================== ofxSQLiteUpdate ==============================

ofxSQLiteUpdate::ofxSQLiteUpdate(sqlite3* pSQLite, std::string sTable) :sqlite(pSQLite), table(sTable) {
}

std::string ofxSQLiteUpdate::getLiteralQuery() {
	std::string sql = "UPDATE " + table + " SET ";

	field_values.begin();
	while (field_values.hasNext()) {
		FieldValuePair field = field_values.current();
		sql += field.field + " = ?" + field.indexString();
		field_values.next();
		sql += (field_values.hasNext() ? "," : "");
	}

	if (wheres.size() > 0) {
		sql += wheres.getLiteralQuery();
	}
	return sql;
}

int ofxSQLiteUpdate::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(), -1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	field_values.bind(statement);
	wheres.bind(statement);

	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	sqlite3_finalize(statement);
	return SQLITE_OK;
}


// ====================== ofxSQLiteDelete ==============================

ofxSQLiteDelete::ofxSQLiteDelete(sqlite3* pSQLite, std::string sTable) :sqlite(pSQLite), table(sTable) {
}

std::string ofxSQLiteDelete::getLiteralQuery(bool bFillValues) {
	std::string sql = "DELETE FROM " + table + " ";
	if (wheres.size() > 0) {
		sql += wheres.getLiteralQuery(bFillValues);
	}
	return sql;
}

int ofxSQLiteDelete::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(), -1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	wheres.bind(statement);

	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	sqlite3_finalize(statement);
	return SQLITE_OK;
}


// ====================== ofxSQLiteSelect ==============================

ofxSQLiteSelect::ofxSQLiteSelect(sqlite3* pSQLite) :sqlite(pSQLite), limit_offset(0), limit_count(0) {
}

ofxSQLiteSelect& ofxSQLiteSelect::select(std::string sFields) {
	fields = sFields;
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::join(std::string sTable, std::string sOnField, std::string sFields) {
	struct Join join = {
		sTable
		, sOnField
		, sFields
	};
	joins.push_back(join);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::order(std::string sField) {
	return order(sField, "ASC");
}

ofxSQLiteSelect& ofxSQLiteSelect::order(std::string sField, std::string sOrder) {
	struct Order order;
	order.field = sField;
	order.order = sOrder;
	orders.push_back(order);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::from(std::string sFrom) {
	from_table = sFrom;
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::limit(int nCount) {
	return limit(nCount, 0);
}

ofxSQLiteSelect& ofxSQLiteSelect::limit(int nCount, int nOffset) {
	limit_count = nCount;
	limit_offset = nOffset;
	return *this;
}

std::string ofxSQLiteSelect::getLiteralQuery(bool bFillValues) {
	// inner joins.
	std::string inner_joins = "";
	for (unsigned i = 0; i < joins.size(); ++i) {
		fields += ", " + joins[i].fields;
		inner_joins = " INNER JOIN " + joins[i].table + " ON " + joins[i].condition;
	}

	// wheres
	std::string where = wheres.getLiteralQuery(bFillValues);

	// limit + offset
	std::string limit = "";
	if (limit_count != 0 && limit_offset == 0) {
		std::stringstream ss_count;
		ss_count << limit_count;
		limit = " LIMIT " + ss_count.str() + " ";
	}
	else if (limit_offset != 0 && limit_count != 0) {
		std::stringstream ss_count, ss_offset;
		ss_count << limit_count;
		ss_offset << limit_offset;
		limit = " LIMIT " + ss_offset.str() + ", " + ss_count.str() + " ";
	}

	// order by
	std::string order_by = "";
	if (orders.size() > 0) {
		order_by += " ORDER BY ";
		for (unsigned i = 0; i < orders.size(); ++i) {
			order_by += " " + orders.at(i).field + " " + orders.at(i).order + ((i < orders.size() - 1) ? ", " : "");
		}
	}

	// combine all sql parts
	std::string sql = " SELECT " + fields
		+ " FROM " + from_table
		+ inner_joins
		+ where
		+ order_by
		+ limit;

	//std::cout << sql << std::endl;
	return sql;
}

ofxSQLiteSelect& ofxSQLiteSelect::execute() {
	std::string sql = getLiteralQuery();
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(), -1, &statement, 0)) {
		sqlite3_finalize(statement);
		std::cout << sqlite3_errmsg(sqlite) << endl;
		return *this;
	}
	wheres.bind(statement);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::begin() {
	col_index = 0;
	last_result = sqlite3_step(statement);
	return *this;
}

int ofxSQLiteSelect::next() {
	last_result = sqlite3_step(statement);
	col_index = 0;
	return last_result;
}

bool ofxSQLiteSelect::hasNext() {
	if (last_result != SQLITE_DONE) {
		return true;
	}
	else {
		sqlite3_finalize(statement);
		return false;
	}
}

bool ofxSQLiteSelect::hasRow() {
	return last_result == SQLITE_ROW;
}

std::string ofxSQLiteSelect::getString(int nIndex) {
	std::string result = "";
	if (last_result != SQLITE_ROW) {
		return result;
	}
	int use_index = nIndex;
	if (use_index == -1)
		use_index = col_index++;

	std::stringstream ss;
	ss << sqlite3_column_text(statement, use_index);
	return ss.str();
}

int ofxSQLiteSelect::getInt(int nIndex) {
	if (last_result != SQLITE_ROW) {
		return 0;
	}
	int use_index = nIndex;
	if (use_index == -1)
		use_index = col_index++;
	return sqlite3_column_int(statement, use_index);
}

float ofxSQLiteSelect::getFloat(int nIndex) {
	if (last_result != SQLITE_ROW) {
		return 0;
	}
	int use_index = nIndex;
	if (use_index == -1)
		use_index = col_index++;
	return sqlite3_column_double(statement, use_index);
}

bool ofxSQLiteSelect::getBool(int nIndex){
	return getInt(nIndex) == 1;
}

std::string ofxSQLiteSelect::getString(string column){
	return getString(getColumnIndex(column));
}

int ofxSQLiteSelect::getInt(string column){
	return getInt(getColumnIndex(column));
}

float ofxSQLiteSelect::getFloat(string column){
	return getFloat(getColumnIndex(column));
}

bool ofxSQLiteSelect::getBool(string column){
	return getBool(getColumnIndex(column));
}


int ofxSQLiteSelect::getNumColumns() {
	return sqlite3_column_count(statement);
}

string ofxSQLiteSelect::getColumnName(int nColumnNum) {
	const char* column_txt = sqlite3_column_name(statement, nColumnNum);
	return column_txt;
}

std::string ofxSQLiteSelect::getResultAsAsciiTable() {
	std::string result;

	// get the biggest width per column and store the results.
	int num_cols = sqlite3_column_count(statement);
	vector<vector<string> > results;
	vector<int> widths;
	widths.assign(num_cols, 0);
	while (sqlite3_step(statement) == SQLITE_ROW) {
		vector<string> row_results;

		for (int i = 0; i < num_cols; ++i) {
			// @todo do we have a memory leak here?
			const unsigned char* column_txt = sqlite3_column_text(statement, i);
			int num_bytes = sqlite3_column_bytes(statement, i);
			string s = "";
			if (num_bytes != 0) {
				s.append((const char*)column_txt, num_bytes);
			}
			//row_results.push_back((unsigned char*)column_txt);
			row_results.push_back(s);
			if ((int)s.size() > widths[i]) {
				widths[i] = s.size() + 2;
			}
		}
		results.push_back(row_results);
	}

	stringstream ss;
	for (unsigned i = 0; i < results.size(); ++i) {
		vector<string> row = results[i];
		for (unsigned c = 0; c < row.size(); ++c) {
			int col_width = widths[c];
			ss << setw(col_width) << row[c] << "  |";
		}
		ss << endl;
	}
	return ss.str();
}

int ofxSQLiteSelect::getColumnIndex(string name){
	for (int i = 0; i < getNumColumns(); i++){
		if (getColumnName(i).compare(name.c_str()) == 0) return i;
	}
	return -1;
}


// ====================== ofxSQLite ==============================

ofxSQLite::ofxSQLite()
:db_name("")
{
}


void ofxSQLite::setup(std::string sDB) {
	db_name = sDB;
	db_file  = ofToDataPath(sDB); 
	if (SQLITE_OK != sqlite3_open(db_file.c_str(), &db)) {
		cout << sqlite3_errmsg(db);
		exit(1);
	}
	cout << "opened:" << db_file << endl;
}	

int ofxSQLite::simpleQuery(const char* pSQL) {
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(db, pSQL,-1, &statement, 0)) {
		return sqlite3_errcode(db);
	}
	while(SQLITE_ROW == sqlite3_step(statement));
	return sqlite3_finalize(statement);
}

ofxSQLiteTable ofxSQLite::createTableIfNotExists(std::string sTable){
	ofxSQLiteTable table = ofxSQLiteTable(db, sTable);
	return table;
}

ofxSQLiteInsert ofxSQLite::insert(std::string sTable) {
	ofxSQLiteInsert insert = ofxSQLiteInsert(db, sTable);
	return insert;
}
ofxSQLiteUpdate ofxSQLite::update(std::string sTable) {
	return ofxSQLiteUpdate(db, sTable);
}

ofxSQLiteDelete ofxSQLite::remove(std::string sTable) {
	return ofxSQLiteDelete(db, sTable);
}

ofxSQLiteSelect	ofxSQLite::select(std::string sFields) {
	ofxSQLiteSelect select = ofxSQLiteSelect(db);
	select.select(sFields);
	return select;
}

const char* ofxSQLite::getError() {
	return sqlite3_errmsg(db);
}

int ofxSQLite::lastInsertID() {
	return sqlite3_last_insert_rowid(db);
}

ofxSQLiteSimpler ofxSQLite::operator[](const std::string sKeyValue)  {
	ofxSQLiteSimpler simple(*this, sKeyValue);
	return simple;
}

void ofxSQLite::printTable(std::string sTable) {
	cout << select("*").from(sTable).execute().getResultAsAsciiTable();
}

#endif