#ifndef SSL_CONNECT_H
#define SSL_CONNECT_H

#include <ssl_base.h>
#include "pop3client.h"
#include <memory>


class ssl_connect : public ssl_base
{
    public:
        ssl_connect(std::string address1, std::string port1);
        std::unique_ptr<pop3client> c2;
        void connect_send (std::string &requesttosend);
        bool connected, authorized;

    protected:

    private:
};

#endif // SSL_CONNECT_H
