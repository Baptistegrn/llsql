#pragma once

#include "llsql/utils/c_cpp.h"

#include <corecrt_math.h>
#include <stdbool.h>
#include <stdio.h>

BEGIN_DECLARATIONS

#define LOG_COLOR_RESET "\033[0m"

#define LOG_COLOR_DEBUG "\033[90m"
#define LOG_COLOR_INFO "\033[32m"
#define LOG_COLOR_WARNING "\033[33m"
#define LOG_COLOR_ERROR "\033[31m"
#define LOG_COLOR_CRITICAL "\033[1;91m"

#define LOG_COLOR_BEGIN "\033[1;36m"
#define LOG_COLOR_INPUT "\033[1;35m"

#define LOG_BEGIN() fprintf(stdout, LOG_COLOR_BEGIN "llsql> " LOG_COLOR_RESET)

#define LOG_DEBUG(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_DEBUG "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                   \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_INFO(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_INFO "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                    \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_WARNING(fmt, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_WARNING "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                 \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_ERROR(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_ERROR "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                   \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_CRITICAL(fmt, ...)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_CRITICAL "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_DEBUG_B(fmt, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        LOG_BEGIN();                                                                                                   \
        fprintf(stdout, LOG_COLOR_DEBUG "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                   \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_INFO_B(fmt, ...)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        LOG_BEGIN();                                                                                                   \
        fprintf(stdout, LOG_COLOR_INFO "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                    \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_WARNING_B(fmt, ...)                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        LOG_BEGIN();                                                                                                   \
        fprintf(stdout, LOG_COLOR_WARNING "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                 \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_ERROR_B(fmt, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        LOG_BEGIN();                                                                                                   \
        fprintf(stdout, LOG_COLOR_ERROR "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                   \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_CRITICAL_B(fmt, ...)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        LOG_BEGIN();                                                                                                   \
        fprintf(stdout, LOG_COLOR_CRITICAL "" fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);                                \
        fflush(stdout);                                                                                                \
    } while (0)

#define LOG_INPUT(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stdout, LOG_COLOR_RESET);                                                                              \
        fflush(stdout);                                                                                                \
    } while (0)

END_DECLARATIONS
