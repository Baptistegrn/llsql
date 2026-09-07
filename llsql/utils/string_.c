#include "llsql/math/types.h"
#include "stdbool.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>

char *StrDupSafe(const char *s)
{
    if (!s)
        return NULL;
    uint64 len = strlen(s) + 1;
    char *copy = malloc(len);
    memcpy(copy, s, len);
    return copy;
}

bool StrCaseCmp(const char *str, const char *prefix)
{
#ifdef _MSC_VER
    while (*prefix)
    {
        if (!*str)
            return false;

        if (tolower((unsigned char)*str) != tolower((unsigned char)*prefix))
            return false;

        str++;
        prefix++;
    }

    return true;
#else
    return strncasecmp(str, prefix, strlen(prefix)) == 0;
#endif
}

char *TrimNewline(char *line)
{
    uint64 len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        line[--len] = '\0';
    return line;
}

char *SkipSpaces(char *s)
{
    while (*s && isspace((unsigned char)*s))
        s++;
    return s;
}