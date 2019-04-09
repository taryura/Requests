#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#include <cstring>
#include <string>
#include "clientBase.h"


class pop3client : public clientBase
{
    public:
        pop3client(boost::asio::io_service& io_service,
          boost::asio::ssl::context& context,
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    protected:
       boost::asio::streambuf MyBuffer;
       void handle_handshake(const boost::system::error_code& error);
       void handle_write(const boost::system::error_code& error,
           size_t bytes_transferred);
       void handle_read(const boost::system::error_code& error,
           size_t bytes_transferred);
       void handle_read_content(const boost::system::error_code& error);
       void handle_read_chunk(const boost::system::error_code& error);
       std::string buff_to_string (boost::asio::streambuf &MyBuffer);
};

#endif // POP3CLIENT_H
