#include "TestData.h"
#include "Utility.h";
#include "Predicates.h";


struct JObject;
struct JArray;

using TagType = std::string;
using JsonType = std::variant<std::string, JObject, JArray, std::int64_t, std::double_t, bool, nullptr_t>;
using TokenMap = std::map<TagType, JsonType>;

JsonType parse_object(const std::string& json, size_t& i);


struct JArray {
    std::vector<JsonType> vec;
};

struct JObject {
    std::map<TagType, JsonType> map;
};


std::string clear_spaces(const std::string& json)
{
    std::string replacement;
    int count = 0;

    for (size_t i = 0; i < json.size(); ++i)
    {
        if ('/' == json[i] && i + 1 < json.size() && '/' == json[i + 1])
        {
            for (; i < json.size() && '\n' != json[i]; ++i);
        }

        if ('"' == json[i] && '\\' != json[i])
        {
            ++count;
            count %= 2;
        }

        if (count || !std::isspace(json[i]))
        {
            replacement += json[i];
        }
    }
    return replacement;
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
        if ('.' == json[i])
        {
            isFloat = true;
        } 
        else if (std::isdigit(json[i]))
        {
            value += json[i];
        }
        else
        {
            throw std::runtime_error("not a literal");
        }
    }
    --i;

    return isFloat 
        ? Utility::str_to_double(value) 
        : Utility::str_to_int64(value);
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

    throw std::runtime_error("not a literal");
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
                vec.emplace_back(std::move(parse_string(json, --i)));
            }

            break;
            case '[':
            {
                vec.emplace_back(std::move(parse_array(json, i)));
                break;
            }
            case '{':
            {
                vec.emplace_back(std::move(parse_object(json, i)));
                break;
            }
            default:
            {
                if (std::isdigit(json[i]))
                {
                    vec.emplace_back(std::move(parse_number(json, --i)));
                }
                else if ('t' == json[i] || 'f' == json[i] || 'n' == json[i])
                {
                    vec.emplace_back(std::move(parse_literal(json, --i)));
                }
                else
                {
                    throw std::runtime_error("error");
                }
            }
        }
    }


    return j;
}

JsonType parse_object(const std::string& json, size_t& i)
{
    std::string tag, value;
    JObject obj;
    TokenMap& map = obj.map;

    for (int j = 0; i < json.size() - 1; i++, j++)
    {
        if (is_object(json[i], json[i + 1]))
        {
            ASSERT(tag.empty(), "tag is empty");
            map[tag] = parse_object(json, i);
        }

        if (is_tag(json[i], json[i + 1])) //is_object üstte olması lazım
        {
            ASSERT(!value.empty(), "value cannot come before");
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

        //if (is_commented_line(json[i], json[i + 1])) //BROKEN
        //{
        //    for (; i < json.size() - 1 && '\n' != json[i]; i++); //Skip function
        //    LOG(json[i]);
        //}

        //if (std::isspace(json[i]))  //BROKEN
        //{
        //    for (; i < json.size() - 1 && !std::isspace(json[i]); i++); //Skip function
        //}
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

    const std::string e = R"({ 
            //"name":"John Smith",
            "age" : 30,"money" : 50.55,
            "isStudent" : false,
            "null" : null 
    })";

      
    JsonType v = tokenize(e);
    JObject o = std::get<JObject>(v);
    for (const auto& [k, v] : o.map)
    {
        std::cout << k << " => ";
        std::visit(PrintVisitor(), v);
    }
    std::cout << std::endl;

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
//Algoritmanın toparlanması lazım tokenların özellikle
//ileri geri atma durumu kaldır switch'e çevir switch yaptıktan sonra bi sonraki değere baksın


//OLASI Json field
//En basiti önce parse'layıp sonra dönüştürülebir.
//Her tip için ayrı vector açıp tutulabilir cache locality için en iyis
//Tagged Union ile 
//std::variant 
//object oriented gidilip her tipe class 