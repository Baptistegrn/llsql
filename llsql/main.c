#include "llsql.h"
#include "llsql/dev/logging.h"
#include "llsql/utils/attributes.h"
#include "llsql/utils/string_.h"
#include <signal.h>

/*
code error :
1:command line error
2:db open error
*/

#define LINE_SIZE 4096

static volatile sig_atomic_t run = 1;

static void HandleSigint(int32 sig)
{
    (void)sig;
    run = 0;
}

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

    signal(SIGINT, HandleSigint);

    sqlite3 *db = DbCreate(argv[1]);

    if (UNLIKELY(db == NULL))
    {
        return 2;
    }

    LOG_INFO("CTRL+C to exit.");

    char line[LINE_SIZE];

    while (run)
    {
        printf("llsql> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            break;
        }

        char *trimmed = SkipSpaces(TrimNewline(line));
        if (*trimmed == '\0')
            continue;

        // Command likes SELECT, etc...
        if (IsDisplayQuery(trimmed))
        {
            LOG_DEBUG("Executing display query: %s", trimmed);

            DbResult res = DbFetch(db, trimmed);

            uint64 nrows = kv_size(res.rows);
            uint64 ncols = 0;

            if (nrows > 0)
            {
                ncols = kv_size(kv_A(res.rows, 0).cols);
            }

            LOG_DEBUG("Query result: %llu row(s), %llu column(s)", (unsigned long long)nrows,
                      (unsigned long long)ncols);

            DbResultPrint(&res);

            LOG_DEBUG("Display query completed.");

            DbResultFree(&res);
        }
        else
        {
            // Command likes INSERT, CREATE, etc...
            LOG_DEBUG("Executing write query: %s", trimmed);

            if (!DbWrite(db, trimmed))
            {
                LOG_ERROR("Writing failed for query: %s", trimmed);
            }
            else
            {
                LOG_DEBUG("Write query completed successfully.");
            }
        }
    }

    LOG_INFO("Data base closing..");
    DbClose(db);
    return 0;
}