#ifndef SSL_BASE_H
#define SSL_BASE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class ssl_base
{
    public:
        ssl_base (std::string address1, std::string port1);


        std::string replyreceived;

    protected:
        void except_handler (std::exception& e);
        boost::asio::io_service io_service;
        boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23};
        boost::asio::ip::tcp::resolver::iterator iterator;

    private:
};

#endif // SSL_BASE_H
