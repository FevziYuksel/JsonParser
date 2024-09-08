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
#include <ranges>
#include <variant>
#include <cinttypes>
#include <functional>

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

    void replace_chars(std::string& str, char oldStr, char newStr)
    {
        std::replace(str.begin(), str.end(), oldStr, newStr);
    }

    int64_t str_to_int64(const std::string& str)
    {
        int64_t num{};
        try {
            num = std::stoll(str);
            //std::cout << "Converted number: " << num << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }

        return num;
    }

    template< typename T>
    std::string num_tostr(T any)
    {
        return std::to_string(any);
    }

    double_t str_to_double(const std::string& str)
    {
        double_t num{};
        try {
            num = std::stod(str);
            //std::cout << "Converted number: " << num << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }

        return num;
    }

    std::string clear_spaces(const std::string& json)
    {
        int strStart = 0;
        int commented = 0;
        std::string replacement;
        for (size_t i = 0; i < json.size(); ++i)
        {
            char c = json[i];
            switch (json[i])
            {
            case '"':
            {
                ++strStart;
                strStart %= 2;
                replacement += json[i];
                break;
            }
            case '/':
            {
                if ('/' == json[++i])
                {
                    for (; '\n' != json[i]; ++i);
                }
                break;
            }
            default:
            {
                if (!std::isspace(json[i]) || 1 == strStart || '"' == json[i])
                {
                    replacement += json[i];
                }
            }
            }
        }
        return replacement;
    }
}

//struct A
//{
//    consteval bool operator==(const A& rhs) 
//    {
//        return true;
//    }
//};

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







