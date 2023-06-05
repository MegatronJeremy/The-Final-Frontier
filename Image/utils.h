#pragma once

#include <c++/9/stdlib.h>
#include <cstdio>
#include <cstring>

// Error utility macro
#define ON_ERROR_EXIT(cond, message) \
do { \
    if((cond)) { \
        printf("Error in function: %s at line %d\n", __func__, __LINE__); \
        perror((message)); \
        exit(1); \
    } \
} while(0)

// Check if a string "str" ends with a substring "ends"
static inline bool str_ends_in(const std::string &str, const std::string &ends) {
    const char *pos = strrchr(str.c_str(), '.');
    return !strcmp(pos, ends.c_str());
}