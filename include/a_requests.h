#ifndef A_REQUESTS_H_INCLUDED
#define A_REQUESTS_H_INCLUDED

#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


enum { max_length = 1024 };

class sslrequest{
public:
    std::string requesttosend, address1, port1, replyreceived;
    void rqst_set (std::string addr,std::string prt,std::string &req_text);
};

class client{
private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  boost::asio::streambuf MyBuffer;
  char reply_[max_length];
  bool verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx);
  void handle_connect(const boost::system::error_code& error);
  void handle_handshake(const boost::system::error_code& error);
  void handle_write(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);

public:
  std::string request_;
  std::string reply2;
  client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1);
};

#endif // A_REQUEST_H_INCLUDED
