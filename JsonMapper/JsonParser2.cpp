#include "TestData.h"
#include "Utility.h";


enum class JsonType : int
{
    OBJECT, ARRAY, STRING, INT, FLOAT, BOOLEAN, JNULL
};

//state machine like

struct Field
{
    JsonType fieldType;
    std::string tag;

    union A
    {
        std::string str;
        long long iNum;
        double fNum;
        bool boolean;
        nullptr_t null;
        A() { memset(this, 0, sizeof(A)); } //neden???
    };
    //union konabilir yada variant
};  

//C style
//struct JObject
//{
//    JsonType fieldType;
//    std::string tag;
//    std::string value;
//};



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


using tokenMap = std::unordered_map< std::string, std::string>;


bool is_object(char first, char second)
{
    return (',' == first && '{' == second);
}

bool is_object(const std::string& token)
{
    return (R"(,{)" == token);
}

bool is_array(char first, char second)
{
    return (':' == first && '[' == second);
}

bool is_array(const std::string& token)
{
    return (R"(:[)" == token);
}

bool is_tag(char first, char second)
{
    return ('{' == first && '"' == second) || (',' == first && '"' == second);
}

bool is_tag(const std::string& token)
{
    return (R"({")" == token || R"(,")" == token);
}

bool is_string(char first, char second)
{
    return (':' == first && '"' == second);
}

bool is_string(const std::string& token)
{
    return (R"(:")" == token);
}

bool is_number(char first, char second)
{
    return (':' == first && std::isdigit(second));
}

bool is_literal(char first, char second)
{
    return (':' == first && ('t' == second || 'f' == second || 'n' == second));
}
//bool is_null(char first, char second)
//{
//    return (':' == first && 'n' == second);
//}

bool end_value(char first)
{
    return (',' == first || '}' == first || ']' == first);
}

bool end_str(char first)
{
    return ('"' != first);
}

void insert2map(tokenMap& map, std::string& tag, std::string& value)
{
    map[tag] = value;
    tag.clear();
    value.clear();
}

std::string parse_tag(const std::string& json, size_t& i)
{
    std::string tag;
    for (i += 2; end_str(json[i]); i++)
    {
        ASSERT(std::isspace(json[i]), "tag cannot contain empty space");
        tag += json[i];
    }
    return tag;
}

std::string parse_string(const std::string& json, size_t& i)
{
    std::string value;
    for (i += 2; end_str(json[i]); i++)
    {
        ASSERT(json.size() == i, "string without \"");
        value += json[i];
    }
    return value;
}

std::string parse_number(const std::string& json, size_t& i)
{
    std::string value;
    for (i += 1; !end_value(json[i]); i++)
    {
        ASSERT(!(std::isdigit(json[i]) || '.' == json[i]), "non numeric value");
        value += json[i];
    }
    --i;
    return value;
}

std::string parse_literal(const std::string& json, size_t& i)
{
    std::string value;
    for (i += 1; !end_value(json[i]); i++)
    {
        value += json[i];
    }
    --i;
    ASSERT(!("true" == value || "false" == value), "not bool");
    ASSERT("null" != value, "not null");
    return value;
}

//std::string parse_null(const std::string& json, size_t& i)
//{
//    std::string value;
//    for (i += 1; !end_value(json[i]); i++)
//    {
//        value += json[i];
//    }
//    --i;
//    ASSERT("null" != value, "not null");
//    return value;
//}

//array'deki virgülleri say vector'ü ona göre aç sonra değerleri move'la
std::string parse_array(const std::string& json, size_t& i)
{
    std::string value;
    std::vector<std::string> vec;
    for (i += 2; ']' != json[i]; i++)
    {
        switch (json[i])
        {
            case '"':
            {
                value = parse_string(json, --i);
                vec.emplace_back(std::move(value));
            }

            break;
            case '[':
            {
                value = parse_array(json, i);
                vec.emplace_back(std::move(value));
                break;
            }
            case '{':
            {
               /* auto m = parse_object(json, i);
                LOG(m);*/
                value = "OBJECT";
                vec.emplace_back(std::move(value));
                break;
            }
            default:
            {
                if (std::isdigit(json[i]))
                {
                    value = parse_number(json, --i);
                    vec.emplace_back(std::move(value));
                }

                if (('t' == json[i] || 'f' == json[i]) || 'n' == json[i])
                {
                    value = parse_literal(json, --i);
                    vec.emplace_back(std::move(value));
                }
            }
        }
    }
    vec.emplace_back(std::move(value));
    //LOG(vec);
    return vec2string(vec);
}

tokenMap parse_object(const std::string& json, size_t& i)
{
    std::string tag, value;
    tokenMap map;

    for (; i < json.size() - 1; i++)
    {
        if (!is_object(json[i], json[i + 1]) && is_tag(json[i], json[i + 1])) //todo tag ile string aynı olabilir 
        {
            ASSERT(!value.empty(), "value cannot be come before");
            tag = parse_tag(json, i);
            //todo Tag'i map'e move'la sonra value'yu
        }

        if (is_string(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            value = parse_string(json, i);
            insert2map(map, tag, value);
        }

        if (is_number(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            value = parse_number(json, i);
            insert2map(map, tag, value);
        }

        if (is_literal(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            value = parse_literal(json, i);
            insert2map(map, tag, value);
        }

        if (is_object(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            auto m = parse_object(json, i);
            LOG(m);
            value = "OBJECT";
            insert2map(map, tag, value);
        }

        if (is_array(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            value = parse_array(json, i);
            insert2map(map, tag, value);
        }
    }

    return map;
}

//{"name":"John Smith","age":30,"money":50.55,"isStudent":false,"null":null}
tokenMap tokenize(const std::string& json)
{
    size_t i = 0;
    return parse_object(json, i);
}


int main()
{
    const std::string exp2 = R"(
    {
      //"1memberShips" : ["a", "b"],
      //"2numbers" : [1, 2, 3, 5],
      "3strVal": "John Smith",
      "4intVal": 30,
      "5floatVal": 50.55,
      "6boolVal": false,
      "7nullVal": null
    })";

    const std::string exp = R"(
    {
        "numbers" : [1, 2, 3, 5],
        "memberShips" : ["fevzi", "yuksel"]
    })";



    std::string v = clear_spaces(exp2);
    LOG(v);
    auto m = tokenize(v);
    LOG(m);
}


// add escape character '\n'

//test single field json for each type
//test simple typed json
//test with array
//test with object

//dönüş değerlerini JToken yap, indexleri de tuple şeklinde dönersin

//JToken base class altına JArray vs 
//Jtoken variant olacak 
//Direk Map için'de tut sonra switch ile ayır (C tarzı)

//C++ string_view kullan substring ile
//C tarzı switch ile char ve pass_empty

//Array bozuk
//Object olmadı
//Düzgün bi şekilde dönüş değeri lazım
//Algoritmanın toparlanması lazım tokenların özellikle
//ileri geri atma durumu kaldır switch'e çevir switch yaptıktan sonra bi sonraki değere baksın