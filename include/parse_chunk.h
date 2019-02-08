#ifndef PARSE_CHUNK_H
#define PARSE_CHUNK_H
#include "reply_parser.h"
#include <iostream>

class parse_chunk : public reply_parser
{
        public:
        //parse_chunk ();
        //parse_chunk (const std::string &response1, int b);
        parse_chunk (const std::string &reply2, int *chunk_start_pointer, int *chunk_length, int &cpi);
        //std::string find_str (const std::string &str_to_find);
        //std::string find_val (const std::string &str_to_find);
        //std::string chop_head (std::string my_chunk, int chunk_length);
        hex_convert (const std::string &my_chunk);
        bool retrieve_chunk_size ();
        std::string string_to_recalc, header, next_chunk, chunk_bytes_to_read, reply2, my_chunk;
        int chunk_bytes, offset_corrector, next_transfer_length;
        bool end_of_chunk, end_of_file, short_string, short_length_data;
        int count_crlf (std::string string_to_count);
        int recalc (std::string reply2, int *chunk_start_pointer, int *chunk_length, int &next_transfer_length, int &cpi);
        void recalc2 (std::string reply2, int *chunk_start_pointer, int *chunk_length, int &next_transfer_length, int &cpi);

    protected:

    private:
        void parse (const std::string &my_chunk);
        int a;
};

#endif // PARSE_CHUNK_H
