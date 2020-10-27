#if !defined(STRING_H)
#define STRING_H

#include <stddef.h>

namespace std {
    size_t strlen(char *str);
    int strcmp(const char *str1, const char *str2);
}

#endif // STRING_H
