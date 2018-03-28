#ifndef SSLREQUEST_H
#define SSLREQUEST_H

#include "a_requests.h"

class sslrequest{
public:
    std::string replyreceived;
    void rqst_set (std::string address1,std::string port1,std::string &requesttosend);
};

#endif // SSLREQUEST_H
