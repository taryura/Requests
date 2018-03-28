#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "a_requests.h"

class httprequest
{
    public:

    void rqst_set (std::string addr, std::string prt, const std::string &req_text);
    std::string replyreceived;

    protected:

    private:
};

#endif // HTTPREQUEST_H
