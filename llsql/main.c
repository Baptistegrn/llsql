#include "llsql.h"
#include "llsql/dev/logging.h"
#include "llsql/utils/attributes.h"
#include "llsql/utils/string_.h"
#include <stdio.h>

/*
code error :
1:command line error
2:db open error
*/

#define LINE_SIZE 4096

static bool IsDisplayQuery(const char *sql)
{
    static const char *kDisplayKeywords[] = {"SELECT", "PRAGMA", "EXPLAIN", "WITH", NULL};

    for (int32 i = 0; kDisplayKeywords[i] != NULL; i++)
    {
        if (StrCaseCmp(sql, kDisplayKeywords[i]))
            return true;
    }
    return false;
}

int32 main(int32 argc, char **argv)
{

    if (argc < 2)
    {
        LOG_ERROR("Usage : ./llsql <databasename>");
        return 1;
    }

    sqlite3 *db = DbCreate(argv[1]);

    if (UNLIKELY(db == NULL))
    {
        return 2;
    }

    LOG_INFO_B("CTRL+C to exit.");

    char line[LINE_SIZE];

    while (true)
    {
        LOG_BEGIN();

        fprintf(stdout, LOG_COLOR_INPUT);
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            fprintf(stdout, LOG_COLOR_RESET);
            fflush(stdout);
            break;
        }

        fprintf(stdout, LOG_COLOR_RESET);
        fflush(stdout);

        char *trimmed = SkipSpaces(TrimNewline(line));

        if (*trimmed == '\0')
            continue;

        // Command like SELECT, etc...
        if (IsDisplayQuery(trimmed))
        {
            LOG_DEBUG_B("Executing display query: %s", trimmed);

            DbResult res = DbFetch(db, trimmed);

            uint64 nrows = kv_size(res.rows);
            uint64 ncols = 0;

            if (nrows > 0)
            {
                ncols = kv_size(kv_A(res.rows, 0).cols);
            }

            LOG_DEBUG_B("Query result: %llu row(s), %llu column(s)", (uint64)nrows, (uint64)ncols);

            DbResultPrint(&res);

            LOG_DEBUG_B("Display query completed.");

            DbResultFree(&res);
        }
        // Command like INSERT, CREATE, etc...
        else
        {
            LOG_DEBUG_B("Executing write query: %s", trimmed);

            if (!DbWrite(db, trimmed))
            {
                LOG_ERROR_B("Writing failed for query: %s", trimmed);
            }
            else
            {
                LOG_DEBUG_B("Write query completed successfully.");
            }
        }
    }

    LOG_INFO("Database closing...");

    DbClose(db);

    return 0;
}