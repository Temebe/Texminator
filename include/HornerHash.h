#ifndef TEXMINATOR_HORNERHASH_H
#define TEXMINATOR_HORNERHASH_H

// Thanks to Youchao
// https://xueyouchao.github.io/2016/11/16/CompileTimeString/

#include <cstdio>

static const size_t PRIME = 31;

template <size_t N>
constexpr inline size_t constHornerHash(const char (&str)[N], size_t Len = N-1) {
    return (Len <= 1) ? str[0] : (PRIME * constHornerHash(str, Len-1) + str[Len-1]);
}

size_t hornerHash(char const* str) {
    if (str == nullptr) {
        return 0;
    }
    size_t hash = *str;
    for (; *(str+1)!=0; str++) {
        hash = PRIME * hash + *(str+1);
    }
    return hash;
}

#endif //TEXMINATOR_HORNERHASH_H
