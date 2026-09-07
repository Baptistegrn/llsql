#pragma once

#include "llsql/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

char *StrDupSafe(const char *s);

bool StrCaseCmp(const char *str, const char *prefix);

char *TrimNewline(char *line);

char *SkipSpaces(char *s);

END_DECLARATIONS