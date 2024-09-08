#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <exception>
#include <cctype>
#include <exception> 
#include <sstream>
#include <algorithm>



const std::string mostComplex = R"(
{
  "address": {
    "street": "123 Main St",
    "city": "New York"
  },
  "memberShips" : ["fevzi", "yuksel"],
  "numbers" : [1, 2, 3, 5],
  "mixList" : [1, "2", [1, 2, 3, 5], {"ob" : 1}],
  "strVal": "John Smith",
  "intVal": 30,
  "floatVal": 50.55,
  "boolVal": false,
  "nullVal": null
})";

const std::string withoutInner = R"(
{
  "strVal": "John Smith",
  "intVal": 30,
  "floatVal": 50.55,
  "boolVal": false,
  "nullVal": null
})";

const std::string arrayTest = R"(
{
 "memberShips" : ["fevzi", "yuksel"],
 "numbers" : [1, 2, 3, 5],
  "bools" : [true, false, null],
"arrays" : [[1, 4], [5, 6, 8]],
 "object" : [{"ob" : 1}, {"ob" : 1}],
  "mixList" : [1, "2", [1, 2, 3, 5], {"ob" : 1}],
})";



const std::array<std::string, 6> singleFieldJsons
{
    R"({"numbers" : [1, 2, 3, 5]})",
    R"("strVal": "John Smith")",
    R"("intVal": 30)",
    R"("floatVal": 50.55)",
    R"("boolVal": false)",
    R"("nullVal": null)",
};