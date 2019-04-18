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


       bool bufferReady, requestInBuffer, terminateIO, IOterminated;

       static std::string readBuffer (std::shared_ptr<std::stringstream> temp_threadBuff_ptr);

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

       std::string buff_to_string (boost::asio::streambuf &MyBuffer);

    private:
        std::shared_ptr<std::stringstream> threadBuff_ptr;
        std::shared_ptr<std::mutex> mtx_ptr;

        //declaring handle methods pointers to change the handlers
        typedef void (pop3client::*HandlerPtr)
                        (const boost::system::error_code& error,
                         size_t bytes_transferred);
        HandlerPtr handler_pointer;

};

#endif // POP3CLIENT_H
