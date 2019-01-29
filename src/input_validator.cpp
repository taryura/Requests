#include "input_validator.h"

input_validator::input_validator (const std::string &str_to_validate)
{
    integer_flag = are_integer (str_to_validate);
    //ctor
}

bool input_validator::are_integer (const std::string &str_to_validate)

{
int length1 = str_to_validate.length();

 for (int i = 0; i < length1; i++)
 {
     int str_byte = str_to_validate[i];

     if (str_byte < 48 or str_byte > 57)
        {
            return 0;
        }
 }
 return 1;
}
