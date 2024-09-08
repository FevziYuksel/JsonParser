#pragma once
#include "Utility.h";

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


bool is_commented_line(char first, char second)
{
    return ('/' == first && '/' == second);
}


