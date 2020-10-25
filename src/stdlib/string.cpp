#include "string.h"

size_t std::strlen(char *str) {
    size_t size = 0;

    while (*str != 0)
        ++size;
    
    return size;
}

int std::strcmp(char *str1, char *str2) {
    int diff = 0;

    while (1) {
        diff = *str1 - *str2;

        if (diff != 0)
            return diff;
        
        if (*str1 == 0)
            return 0;
        
        ++str1;
        ++str2;
    }
}