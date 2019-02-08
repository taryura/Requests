#include "parse_head.h"


/*parse_head::parse_head ()
{

}*/

parse_head::parse_head(const std::string &response1)
{
   header = response1;

   int head_end = header.find("\r\n\r\n");
   if (head_end != -1)
   {
       next_chunk = chop_head(header, (head_end+4));
   }
   else
   {
       err = 1;
   }
   //err = retrieve_chunk_size();

}
// finds and returns text right after the header "\r\n\r\n"

std::string parse_head::chop_head (std::string my_chunk, int chunk_length)
{
    std::string string1 = my_chunk.substr(chunk_length, (my_chunk.length() - chunk_length));
    return string1;
}


//finds and returns string which begins with str_to_find

std::string parse_head::find_str (const std::string &str_to_find)
{
    int a = header.find (str_to_find);

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
std::string parse_head::find_val (const std::string &str_to_find)
{
    a = str_to_find.find (":")+2;
    std::string string2 = str_to_find.substr(a, str_to_find.length()-a);

    return string2;
}
