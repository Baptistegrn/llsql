#pragma once

// TODO : use haclog for colors

#include "llsql/utils/c_cpp.h"

#define LOG_OUT stdout

BEGIN_DECLARATIONS

#define LOG_ERROR(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(LOG_OUT, "[ERROR] " fmt "\n", ##__VA_ARGS__);                                                          \
        fflush(LOG_OUT);                                                                                               \
    } while (0)

#define LOG_WARN(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(LOG_OUT, "[WARN] " fmt "\n", ##__VA_ARGS__);                                                           \
        fflush(LOG_OUT);                                                                                               \
    } while (0)

#define LOG_INFO(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(LOG_OUT, "[INFO] " fmt "\n", ##__VA_ARGS__);                                                           \
        fflush(LOG_OUT);                                                                                               \
    } while (0)

#define LOG_DEBUG(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(LOG_OUT, "[DEBUG] " fmt "\n", ##__VA_ARGS__);                                                          \
        fflush(LOG_OUT);                                                                                               \
    } while (0)

END_DECLARATIONS