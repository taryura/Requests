#ifndef CLIENTBASE_H
#define CLIENTBASE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


class clientBase
{
    public:
      clientBase(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1);
      std::string reply2, request_;

    protected:
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    bool verify_certificate(bool preverified,
        boost::asio::ssl::verify_context& ctx);
    void handle_connect(const boost::system::error_code& error);
    virtual void handle_handshake(const boost::system::error_code& error) =0;

    private:


};

#endif // CLIENTBASE_H
