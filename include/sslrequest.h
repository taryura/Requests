#ifndef SSLREQUEST_H
#define SSLREQUEST_H

#include <ssl_base.h>
#include "a_requests.h"

class sslrequest : public ssl_base

{
public:

    sslrequest (std::string address1, std::string port1, std::string &requesttosend);

private:

};

#endif // SSLREQUEST_H
