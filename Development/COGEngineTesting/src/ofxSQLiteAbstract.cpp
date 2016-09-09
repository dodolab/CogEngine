#include "ofxSQLiteAbstract.h"
#include "ofxSQLiteFieldValues.h"
#include "ofxsqlite3.h"

ofxSQLiteAbstract::ofxSQLiteAbstract(sqlite3* pSQLite):sqlite(pSQLite) {
}
