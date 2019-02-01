#include <iostream>

class head_type
{
    public:
        head_type ();
        head_type (const std::string &response1);
        std::string find_str (const std::string &str_to_find);
        std::string find_val (const std::string &str_to_find);
        std::string chop_head (std::string my_chunk, int chunk_length);
        hex_convert (const std::string &my_chunk);
        bool retrieve_chunk_size ();
        std::string header, next_chunk, chunk_bytes_to_read;
        int chunk_bytes;
        bool err;

    protected:

    private:
        int a;
};

