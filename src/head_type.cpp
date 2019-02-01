#include "head_type.h"

head_type::head_type ()
{

}

head_type::head_type(const std::string &response1)
{
   header = response1;
   next_chunk = chop_head(header, (header.find("\r\n\r\n")+4));
   err = retrieve_chunk_size();

}
bool head_type::retrieve_chunk_size ()

{
   chunk_bytes_to_read = next_chunk.substr(0, next_chunk.find("\r\n"));
   chunk_bytes = hex_convert(chunk_bytes_to_read);
   if (chunk_bytes == -1){return 1;}
   else {return 0;}
}
//Converts std::string (hex) to unsigned int
int head_type::hex_convert (const std::string &my_chunk)
{
    try
    {
       unsigned int x = std::stoul(my_chunk, nullptr, 16);
       return x;
    }
    catch (...)
    {
        return -1;
    }
}




// finds and returns text right after the header "\r\n\r\n"
std::string head_type::chop_head (std::string my_chunk, int chunk_length)
{
    std::string string1 = my_chunk.substr(chunk_length, (my_chunk.length() - chunk_length));
    return string1;
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
