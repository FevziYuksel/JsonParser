// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Bitmedi Çok fazla bug çıktığı için bıraktım

#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <cctype>

#include "TestData.h"

//enum class JsonType : int
//{
//    OBJECT, ARRAY, STRING, INT, FLOAT, BOOLEAN, JNULL
//};

//state machine like

//struct Field
//{
//    JsonType fieldType;
//    std::string tag;
//
//    union
//    {
//        std::string str;
//        long long iNum;
//        double fNum;
//        bool boolean;
//        nullptr_t null;
//        //A() { memset(this, 0, sizeof(A)); } //neden???
//    } field;
//    //union konabilir yada variant
//};  

//C style
//struct Field
//{
//    JsonType fieldType;
//    std::string tag;
//    std::string value;
//};


static std::map<std::string, std::string> json_mapper(const std::string& json);


static const char* specialTokens[] = {"null" , "true", "false"};

static void special_token(const std::string& json, size_t& i, bool& isTag, std::string& tag, std::string& value, std::map<std::string, std::string>& tagValue)
{
    size_t len = json[i] == 'f' ? 5 : 4;

    size_t j = i;
    for (; j < j + len && j < json.size(); ++j)
    {
        value += json[j];
    }

    for (const auto& st : specialTokens)
    {
        if (value == st) 
        {
            i = j;
            tagValue[tag] = value;
            tag.clear();
            value.clear();
            isTag = true;
            return;
        }
    }
   
    char x = json[i];
    throw std::runtime_error{ "special token error" };
}

static std::map<std::string, std::string> json_mapper(const std::string& json)
{
    std::map<std::string, std::string> tagValue;
    bool isTag = true;
    std::string tag, value;
    for (size_t i = 0; i < json.size(); ++i)
    {
        char c = json[i];

        switch (json[i])
        {
        case ' ':
        case '\n':
        case '\t':
            break;
        case '{':
        case '}':
            break;
        case ':':
            isTag = false;
            break;
        case ',':
            tag.clear();
            value.clear();
            isTag = true;
            break;
        case '"':
            if (isTag)
            {
                size_t j = i + 1;
                for (; json[j] != '"' && j < json.size(); ++j)
                {
                    tag += json[j];
                }
                i = j;
                isTag = false;
                break;
            }
            else
            {
                size_t j = i + 1;
                for (; json[j] != '"' && j < json.size(); ++j)
                {
                    value += json[j];
                }
                i = j;
                tagValue[tag] = value;
                tag.clear();
                value.clear();
                isTag = true;
                break;
            }
            break;
        default:
            switch (json[i])
            {
                case 'n':
                case 'f':
                case 't':
                    special_token(json, i, isTag, tag, value, tagValue);
                    break;
                default:
                    if (std::isdigit(json[i]) || json[i] == '.')
                    {
                        size_t j = i;
                        for (; json[j] != ',' && j < json.size(); ++j)
                        {
                            if (std::isdigit(json[i]) || json[i] == '.')
                            {
                                value += json[j];
                            }
                        }
                        i = j;
                        tagValue[tag] = value;
                        tag.clear();
                        value.clear();
                        isTag = true;
                    }
                    break;
                    char x = json[i];
                    throw std::runtime_error{ "token error3" };
            }
        }
    }
    return tagValue;
}




