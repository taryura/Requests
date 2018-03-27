#include "head_type.h"

head_type::head_type(const std::string &response1)
{
   header = response1;
}

//finds and returns string which begins with str_to_find

std::string head_type::find_str (const std::string &str_to_find)
{
    a = header.find (str_to_find);

    if (a !=-1){
            std::string string2 = header.substr(a, header.length()-a);
            a = string2.find ("\r\n");
            std::string string3 = string2.substr(0, a);
            err = 0;
            return string3;
    }
    err = 1;
    return "Cannon find element";
}
//returns the value after the colon
std::string head_type::find_val (const std::string &str_to_find)
{
    a = str_to_find.find (":")+2;
    std::string string2 = str_to_find.substr(a, str_to_find.length()-a);

    return string2;
}
