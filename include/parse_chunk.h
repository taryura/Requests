#ifndef PARSE_CHUNK_H
#define PARSE_CHUNK_H
#include <iostream>
#include "head_type.h"


class parse_chunk : public head_type
{
        public:
        parse_chunk (const std::string &my_chunk);
        int count_crlf (std::string string_to_count);

    protected:

    private:
};

#endif // PARSE_CHUNK_H
