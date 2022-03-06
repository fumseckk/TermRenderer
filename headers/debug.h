#ifndef DEBUG_H
#define DEBUG_H


#ifndef NDEBUG

#include <stdio.h>
#include <time.h>


#define _EPRINTF(str...) fprintf(stderr, str)


#define _LOG_HEADER(header) ({                                      \
    time_t timer = time(NULL);                                      \
    struct tm* t = localtime(&timer);                               \
    _EPRINTF("<%i:%i:%i> [%s - " __FILE__ ":%i] ",                  \
            t->tm_hour, t->tm_min, t->tm_sec, header, __LINE__);    \
})


#define WARNING(str...) ({      \
    _LOG_HEADER("WARNING");     \
    _EPRINTF(str);              \
    _EPRINTF("\n");             \
})


#define FATAL_ERROR(cond, err...) ({        \
    if (cond) {                             \
        _LOG_HEADER("FATAL ERROR");         \
        _EPRINTF(err);                      \
        _EPRINTF("\n");                     \
        exit(EXIT_FAILURE);                 \
    }                                       \
})


#define PRINT(str...) ({        \
    _LOG_HEADER("out");         \
    _EPRINTF(str);              \
    _EPRINTF("\n");             \
})


#else // #ifndef NDEBUG

#define WARNING(str...)
#define FATAL_ERROR(cond, err...)
#define PRINT(str...)

#endif // #ifndef NDEBUG


#endif // #ifndef DEBUG_H