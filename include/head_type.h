#include <iostream>

class head_type
{
    public:
        head_type (const std::string &response1);
        std::string find_str (const std::string &str_to_find);
        std::string find_val (const std::string &str_to_find);
        std::string header;
        bool err;

    protected:

    private:
        int a;
};
