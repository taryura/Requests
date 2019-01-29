#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H
#include <iostream>

class input_validator
{
    public:
        input_validator (const std::string &str_to_validate);
        bool integer_flag;
        bool are_integer (const std::string &str_to_validate);

    protected:

    private:
};

#endif // INPUT_VALIDATOR_H
