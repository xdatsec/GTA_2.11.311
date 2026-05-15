#pragma once

#include <string>
#include <iostream>
#include <iterator>

//хранилице зашифрованных строк
template<std::size_t SIZE>
struct hiddenString
{
    //буффер для зашифрованной строки
    short s[SIZE];

    //конструктор для создания объекта на этапе компиляции
    constexpr hiddenString():s{0} { }

    //функция дешифрации в процессе исполнения приложения
    std::string decode() const
    {
        std::string rv;
        rv.reserve(SIZE + 1);
        std::transform(s, s + SIZE - 1, std::back_inserter(rv), [](auto ch) {
            return ch - 1;
        });
        return rv;
    }
};

//вычисление размера строки на этапе компиляции

template<typename T, std::size_t N> constexpr std::size_t sizeCalculate(const T(&)[N])
{
    return N;
}


//функция шифрации на этапе компиляции
template<std::size_t SIZE>
constexpr auto encoder(const char str[SIZE])
{
    hiddenString<SIZE> encoded;
    for(std::size_t i = 0; i < SIZE - 1; i++)
        encoded.s[i] = str[i] + 1;
    encoded.s[SIZE - 1] = 0;
    return encoded;
}

//макрос для удобства использования
#define CRYPTEDSTRING(name, x) constexpr auto name = encoder<sizeCalculate(x)>(x)