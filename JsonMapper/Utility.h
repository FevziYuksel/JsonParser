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

    std::string int64_to_str(int64_t num)
    {
        return std::to_string(num);
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

    std::string double_to_str(double_t num)
    {
        return std::to_string(num);
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




