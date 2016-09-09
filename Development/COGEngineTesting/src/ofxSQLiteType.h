#ifndef OFXSQLITETYPEH
#define OFXSQLITETYPEH
#include <string>

enum {
 	 OFX_SQLITE_TYPE_INT
	,OFX_SQLITE_TYPE_DOUBLE
	,OFX_SQLITE_TYPE_LONG
	,OFX_SQLITE_TYPE_INT64
	,OFX_SQLITE_TYPE_TEXT
	,OFX_SQLITE_TYPE_NULL
};

enum {
	SQLITE_COLUMN_INTEGER,
	SQLITE_COLUMN_VARCHAR,
	SQLITE_COLUMN_BOOLEAN,
	SQLITE_COLUMN_DATETIME,
	SQLITE_COLUMN_TEXT
};

class ofxSQLiteType {
public:
	virtual std::string getString()		{	return	"";		}
	virtual int getInt()				{ 	return	0;		}	
	virtual long getLong()				{ 	return	0;		}
	virtual uint64_t getUint64()		{	return  0;		}
	virtual int getType() = 0;
};
#endif
