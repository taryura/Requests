#include "reply_parser.h"
#include <iostream>

class parse_head : public reply_parser
{
    public:
        //head_type ();
        parse_head (const std::string &response1);
        std::string find_str (const std::string &str_to_find);
        std::string find_val (const std::string &str_to_find);
        std::string chop_head (std::string my_chunk, int chunk_length);
        std::string header;
        int chunk_bytes;

    protected:

    private:

        int a;
};

