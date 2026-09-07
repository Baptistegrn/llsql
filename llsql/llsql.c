#include "llsql.h"
#include "kvec.h"
#include "llsql/dev/logging.h"
#include "llsql/utils/attributes.h"
#include "llsql/utils/string_.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sqlite3_stmt *DbPrepareInternal(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        LOG_ERROR("%s", sqlite3_errmsg(db));
    return stmt;
}

static bool DbExists(const char *name)
{
    FILE *fs = fopen(name, "r");

    if (fs == NULL)
        return false;

    fclose(fs);
    return true;
}

sqlite3 *DbCreate(const char *name)
{
    if (!DbExists(name))
    {
        LOG_WARN("%s", "Database does not exist, creating one.");
    }
    sqlite3 *db = NULL;
    if (UNLIKELY(sqlite3_open(name, &db) != SQLITE_OK))
    {
        LOG_ERROR("%s", name ? sqlite3_errmsg(db) : "unknown error");
        if (db)
            sqlite3_close(db);
        return NULL;
    }
    return db;
}

void DbClose(sqlite3 *db)
{
    sqlite3_close(db);
}

bool DbWrite(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = DbPrepareInternal(db, sql);
    if (!stmt)
        return false;

    int32 rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        LOG_ERROR("%s", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

static DbResult DbResultGet(sqlite3_stmt *stmt)
{
    DbResult result;
    kv_init(result.rows);

    int32 colCount = sqlite3_column_count(stmt);
    int32 rc;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        DbRow row;
        kv_init(row.cols);

        for (int32 i = 0; i < colCount; i++)
        {
            DbValue v = {0};
            v.name = StrDupSafe(sqlite3_column_name(stmt, i));

            switch (sqlite3_column_type(stmt, i))
            {
            case SQLITE_INTEGER:
                v.type = INTEGER;
                v.value.i = sqlite3_column_int64(stmt, i);
                break;
            case SQLITE_FLOAT:
                v.type = FLOAT;
                v.value.f = sqlite3_column_double(stmt, i);
                break;
            case SQLITE_TEXT:
                v.type = TEXT;
                v.value.s = StrDupSafe((const char *)sqlite3_column_text(stmt, i));
                break;
            case SQLITE_BLOB: {
                v.type = DATA;
                int32 size = sqlite3_column_bytes(stmt, i);
                v.value.blob.data = malloc(size);
                memcpy(v.value.blob.data, sqlite3_column_blob(stmt, i), size);
                v.value.blob.size = size;
                break;
            }
            case SQLITE_NULL:
                v.type = VOID;
                break;
            }

            kv_push(DbValue, row.cols, v);
        }

        kv_push(DbRow, result.rows, row);
    }

    if (rc != SQLITE_DONE)
        LOG_ERROR("%s", sqlite3_errmsg(sqlite3_db_handle(stmt)));

    return result;
}

DbResult DbFetch(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = DbPrepareInternal(db, sql);
    if (!stmt)
        return (DbResult){0};
    DbResult res = DbResultGet(stmt);
    sqlite3_finalize(stmt);
    return res;
}

// TODO: improve format system and add colors
static int FormatDbValue(const DbValue *v, char *buf, uint64 bufsize)
{
    switch (v->type)
    {
    case INTEGER:
        return snprintf(buf, bufsize, "%lld", (long long)v->value.i);

    case FLOAT:
        return snprintf(buf, bufsize, "%g", v->value.f);

    case TEXT:
        return snprintf(buf, bufsize, "%s", v->value.s ? v->value.s : "");

    case DATA:
        return snprintf(buf, bufsize, "[BLOB %llu octets]", (unsigned long long)v->value.blob.size);

    case VOID:
        return snprintf(buf, bufsize, "NULL");

    default:
        return snprintf(buf, bufsize, "NULL");
    }
}

void DbResultPrint(DbResult *res)
{
    uint64 nrows = kv_size(res->rows);
    if (nrows == 0)
    {
        return;
    }
    DbRow first = kv_A(res->rows, 0);
    uint64 ncols = kv_size(first.cols);
    if (ncols == 0)
    {
        return;
    }
    for (uint64 r = 1; r < nrows; r++)
    {
        if (kv_size(kv_A(res->rows, r).cols) != ncols)
        {
            return;
        }
    }
    const uint64 kValueBufSize = 1024;
    uint64 *widths = malloc(ncols * sizeof(uint64));
    char **formatted = malloc(nrows * ncols * sizeof(char *));
    for (uint64 c = 0; c < ncols; c++)
    {
        widths[c] = strlen(kv_A(first.cols, c).name);
    }
    for (uint64 r = 0; r < nrows; r++)
    {
        DbRow row = kv_A(res->rows, r);
        for (uint64 c = 0; c < ncols; c++)
        {
            DbValue v = kv_A(row.cols, c);
            char *buf = malloc(kValueBufSize);
            int32 len = FormatDbValue(&v, buf, kValueBufSize);
            if (len < 0)
                len = 0;
            formatted[r * ncols + c] = buf;
            if ((uint64)len > widths[c])
                widths[c] = (uint64)len;
        }
    }

    /* En-tête */
    for (uint64 c = 0; c < ncols; c++)
    {
        printf("%-*s", (int32)widths[c], kv_A(first.cols, c).name);

        if (c + 1 < ncols)
            printf(" | ");
    }

    printf("\n");
    for (uint64 c = 0; c < ncols; c++)
    {
        for (uint64 i = 0; i < widths[c]; i++)
            putchar('-');

        if (c + 1 < ncols)
            printf("-+-");
    }
    printf("\n");
    for (uint64 r = 0; r < nrows; r++)
    {
        for (uint64 c = 0; c < ncols; c++)
        {
            printf("%-*s", (int32)widths[c], formatted[r * ncols + c]);

            if (c + 1 < ncols)
                printf(" | ");

            free(formatted[r * ncols + c]);
        }

        printf("\n");
    }
    free(formatted);
    free(widths);
}

void DbResultFree(DbResult *res)
{
    for (uint64 r = 0; r < kv_size(res->rows); r++)
    {
        DbRow row = kv_A(res->rows, r);
        for (uint64 c = 0; c < kv_size(row.cols); c++)
        {
            DbValue v = kv_A(row.cols, c);
            free((void *)v.name);
            if (v.type == TEXT)
                free((void *)v.value.s);
            if (v.type == DATA)
                free(v.value.blob.data);
        }
        kv_destroy(row.cols);
    }
    kv_destroy(res->rows);
}
