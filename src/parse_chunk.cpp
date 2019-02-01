#include "parse_chunk.h"

parse_chunk::parse_chunk (const std::string &my_chunk)
{
    next_chunk = my_chunk;
    std::cout << "Next part beggining " << next_chunk << std::endl;
    int a = count_crlf (my_chunk);
    std::cout << "The number of crlf " << a << std::endl;
    if (a < 2){err = 1;}
    else {err = 0;}
    std::cout << "err " << err << std::endl;
    if (!err)
    {
        next_chunk = next_chunk.substr(2);
        err = retrieve_chunk_size();
    }

}

int parse_chunk::count_crlf (std::string string_to_count)
{
    //set the counter to 0
    int count1 = 0;
    //define offset of the first crlf (.find returns -1 if there isn't any)
    int crlf_offset = string_to_count.find("\r\n");
    if (crlf_offset == -1)
        {
            return count1;
        }
    else
        {
         do
         {
             count1 +=1;
             string_to_count = string_to_count.substr(crlf_offset +2);
             crlf_offset = string_to_count.find("\r\n");
         }
         while (crlf_offset != -1);
         return count1;
        }


}
