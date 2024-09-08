#pragma once
#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>
#include <type_traits>

#include "GenericPrinter.h"

#define LOG(X) (std:: cout << X << "\n")

namespace Utility {
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(s);
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void replaceChars(std::string& str, char oldStr, char newStr)
    {
        std::replace(str.begin(), str.end(), oldStr, newStr);
    }
}

//struct A
//{
//    consteval bool operator==(const A& rhs) 
//    {
//        return true;
//    }
//};

//Copy ??
template<typename T>
constexpr bool contains(const std::initializer_list<T>& arr, T obj)
{
    for (const auto& var : arr)
    {
        if (var == obj)
        {
            return true;
        }
    }
    return false;
}

#define LOG(X) (std:: cout << X << "\n")

#define ASSERT(CASE, TEXT) if (CASE) {std::cerr << TEXT << "\n";}


consteval const char* bool2string(bool val)
{
    return (val ? "true" : "false");
}

std::string vec2string(const std::vector<std::string>& vec)
{
    std::stringstream ss;
    for (const auto& s : vec)
    {
        ss << s << ",";
    }
    return ss.str();
}
