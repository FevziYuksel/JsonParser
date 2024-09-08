#include "TestData.h"
#include "Utility.h";

struct JObject;
struct JArray;

using TagType = std::string;
using JsonType = std::variant<std::string, JObject, JArray, std::int64_t, std::double_t, bool, nullptr_t>;
using TokenMap = std::map<TagType, JsonType>;


struct JArray {
    std::vector<JsonType> vec;
};

struct JObject {
    std::map<TagType, JsonType> map;
};


std::variant<std::string, JObject, JArray, std::int64_t, std::double_t, bool, nullptr_t> tokens;



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


bool is_object(char first, char second)
{
    return (',' == first && '{' == second);
}

bool is_array(char first, char second)
{
    return (':' == first && '[' == second);
}


bool is_tag(char first, char second)
{
    return ('{' == first && '"' == second) || (',' == first && '"' == second);
}


bool is_string(char first, char second)
{
    return (':' == first && '"' == second);
}

bool is_number(char first, char second)
{
    return (':' == first && std::isdigit(second));
}

bool is_literal(char first, char second)
{
    return (':' == first && ('t' == second || 'f' == second || 'n' == second));
}

bool end_value(char first)
{
    return (',' == first || '}' == first || ']' == first);
}

bool end_str(char first)
{
    return ('"' != first);
}


TagType parse_tag(const std::string& json, size_t& i)
{
    std::string tag;
    for (i += 2; end_str(json[i]); i++)
    {
        ASSERT(std::isspace(json[i]), "tag cannot contain empty space");
        tag += json[i];
    }
    return tag;
}

JsonType parse_string(const std::string& json, size_t& i)
{
    std::string value;
    for (i += 2; end_str(json[i]); i++)
    {
        ASSERT(json.size() == i, "string without \"");
        value += json[i];
    }
    return value;
}

JsonType parse_number(const std::string& json, size_t& i)
{
    std::string value;
    bool isFloat = false;

    for (i += 1; !end_value(json[i]); i++)
    {
        ASSERT(!(std::isdigit(json[i]) || '.' == json[i]), "non numeric value");
        if ('.' == json[i])
        {
            isFloat = true;
        }

        value += json[i];
    }
    --i;

    return isFloat ? Utility::str_to_double(value) : Utility::str_to_int64(value);
}

JsonType parse_literal(const std::string& json, size_t& i)
{
    std::string value;
    for (i += 1; !end_value(json[i]); i++)
    {
        value += json[i];
    }
    --i;
    
    if ("true" == value)
    {
        return true;
    } 

    if ("false" == value)
    {
        return false;
    }

    if ("null" == value)
    {
        return nullptr;
    }

    throw std::runtime_error("");
}


//array'deki virgülleri say vector'ü ona göre aç sonra değerleri move'la
JsonType parse_array(const std::string& json, size_t& i)
{
    JsonType value;
    JArray j;
    std::vector<JsonType>& vec = j.vec;
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
    return j;
}

JsonType parse_object(const std::string& json, size_t& i)
{
    std::string tag, value;
    JObject obj;
    TokenMap& map = obj.map;

    for (; i < json.size() - 1; i++)
    {
        if (is_object(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            map[tag] = parse_object(json, i);
        }

        if (is_tag(json[i], json[i + 1])) //todo tag ile string aynı olabilir 
        {
            ASSERT(!value.empty(), "value cannot be come before");
            tag = parse_tag(json, i);
            //todo Tag'i map'e move'la sonra value'yu
        }

        ASSERT(tag.empty(), "tag is empty");

        if (is_string(json[i], json[i + 1]))
        {
            map[tag] = parse_string(json, i);
        }

        if (is_number(json[i], json[i + 1]))
        {
            map[tag] = parse_number(json, i);
        }

        if (is_literal(json[i], json[i + 1]))
        {
            map[tag] = parse_literal(json, i);;
        }

        if (is_array(json[i], json[i + 1]))
        {
            map[tag] = parse_array(json, i);
        }
    }

    return obj;
}

//{"name":"John Smith","age":30,"money":50.55,"isStudent":false,"null":null}
JsonType tokenize(const std::string& json)
{
    size_t i = 0;
    return parse_object(clear_spaces(json), i);
}

void print_all(JsonType v);

struct PrintVisitor {
    void operator()(nullptr_t value) const {
        std::cout << "null: " << value << "\n";
    }

    void operator()(bool value) const {
        std::cout << "bool: " << value << "\n";
    }

    void operator()(int64_t value) const {
        std::cout << "int64: " << value << "\n";
    }

    void operator()(double_t value) const {
        std::cout << "double: " << value << "\n";
    }

    void operator()(const std::string& value) const {
        std::cout << "string: " << value << "\n";
    }

    void operator()(const JArray& value) const {
        std::cout << "array: " << "\n";
        //for (const auto& e : value.vec)
        //{
        //    std::visit(PrintVisitor(), e);
        //}
    }

    void operator()(const JObject& value) const {
        std::cout << "object: " << std::endl;
        //for (const auto& [k, v] : value.map)
        //{
        //    print_all(value);
        //}
    }
};

void print_all(JsonType v)
{
    std::visit(PrintVisitor(), v);
}


int main()
{
    const std::string exp2 = R"(
    {
      "1memberShips" : ["a", "b"],
      "2numbers" : [1, 2, 3, 5],
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


    JsonType v = tokenize(R"({ "name":"John Smith","age" : 30,"money" : 50.55,"isStudent" : false,"null" : null })");
    JObject o = std::get<JObject>(v);
    for (const auto& [k, v] : o.map)
    {
        std::cout << k << " => ";
        std::visit(PrintVisitor(), v);
    }
    std::cout << std::endl;


    //LOG(m);
    return 0;
}


// add escape character '\n'

//test single field json for each type
//test simple typed json
//test with array
//test with object

//dönüş değerlerini JToken yap, indexleri de tuple şeklinde dönersin

//C++ string_view kullan substring ile
//C tarzı switch ile char ve pass_empty

//Array bozuk
//Object olmadı
//Düzgün bi şekilde dönüş değeri lazım
//Algoritmanın toparlanması lazım tokenların özellikle
//ileri geri atma durumu kaldır switch'e çevir switch yaptıktan sonra bi sonraki değere baksın


//OLASI Json field
//En basiti önce parse'layıp sonra dönüştürülebir.
//Her tip için ayrı vector açıp tutulabilir cache locality için en iyis
//Tagged Union ile 
//std::variant 
//object oriented gidilip her tipe class 