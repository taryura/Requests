#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#include <memory>
#include <mutex>
#include <cstring>
#include <string>
#include "clientBase.h"


class pop3client : public clientBase
{
    public:
        pop3client(boost::asio::io_service& io_service,
          boost::asio::ssl::context& context,
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
          std::shared_ptr<std::stringstream> stringstream_ptr,
          std::shared_ptr<std::mutex> mutex_ptr);
       void write_req(std::string &request_to_write);
       bool bufferReady, requestInBuffer;
       //boost::system::error_code *get_error();

    protected:
       const boost::system::error_code *e2;
       boost::asio::streambuf MyBuffer;
       void handle_handshake(const boost::system::error_code& error);
       void handle_write(const boost::system::error_code& error,
                         size_t bytes_transferred);
       void handle_write2(const boost::system::error_code& error);
       void handle_read(const boost::system::error_code& error,
           size_t bytes_transferred);
        void handle_read2(const boost::system::error_code& error,
                          size_t bytes_transferred);
       //void handle_read_content(const boost::system::error_code& error);
       //void handle_read_chunk(const boost::system::error_code& error);
       std::string buff_to_string (boost::asio::streambuf &MyBuffer);
    private:
        std::shared_ptr<std::stringstream> threadBuff_ptr;
        std::shared_ptr<std::mutex> mtx_ptr;
        std::string readBuffer ();
};

#endif // POP3CLIENT_H
