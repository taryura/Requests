#ifndef SSL_CONNECT_H
#define SSL_CONNECT_H
#include <thread>
#include <memory>
#include <mutex>
#include <ssl_base.h>
#include "pop3client.h"




class ssl_connect : public ssl_base
{
    public:

        ssl_connect(std::string address1, std::string port1);
        void connect_send (std::string &requesttosend);
        bool connected, authorized;
        void client_run (std::shared_ptr<std::stringstream> stringstream_ptr,
                              std::shared_ptr<std::mutex> mutex_ptr);
        void SetMessSent();
        bool ReadyToRead();

        std::shared_ptr<std::stringstream> threadBuff_ptr;
        std::shared_ptr<std::mutex> mtx_ptr;


    protected:

    private:
        std::thread t1;
        std::unique_ptr<pop3client> c2;
        //std::string readBuffer ();

};

#endif // SSL_CONNECT_H
