#pragma once

#include "kvec.h"
#include "llsql/math/types.h"
#include "llsql/utils/c_cpp.h"
#include <sqlite3.h>
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef enum
{
    INTEGER = 0,
    FLOAT = 1,
    TEXT = 2,
    DATA = 3,
    VOID = 4
} DbType;

typedef struct
{
    const char *name; // null for DbArg
    DbType type;
    union {
        int64 i;
        float64 f;
        const char *s;
        struct
        {
            void *data;
            uint64 size;
        } blob;
    } value;
} DbValue;

typedef struct
{
    kvec_t(DbValue) cols;
} DbRow;

typedef struct
{
    kvec_t(DbRow) rows;
} DbResult;

typedef DbValue DbArg;

sqlite3 *DbCreate(const char *name);
void DbClose(sqlite3 *db);

void DbResultPrint(DbResult *res);
void DbResultFree(DbResult *res);

bool DbWrite(sqlite3 *db, const char *sql);
DbResult DbFetch(sqlite3 *db, const char *sql);

END_DECLARATIONS
