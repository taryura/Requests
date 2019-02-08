#ifndef REPLY_PARSER_H
#define REPLY_PARSER_H
#include <iostream>

class reply_parser
{
    public:
        reply_parser();
        //reply_parser(const std::string &reply2);
        bool err;
        std::string next_chunk;

    protected:

    private:
};

#endif // REPLY_PARSER_H
