#ifndef SSLREQUEST_H
#define SSLREQUEST_H

#include "a_requests.h"

class sslrequest{
public:
    std::string replyreceived;
    void connect_set (std::string &requesttosend);
    void connect_set ();
    sslrequest (std::string address1, std::string port1);

private:
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver::iterator iterator;
        void except_handler (std::exception& e);
};

#endif // SSLREQUEST_H
