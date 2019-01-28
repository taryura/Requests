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


class client{
private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  boost::asio::streambuf MyBuffer;
  bool verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx);
  void handle_connect(const boost::system::error_code& error);
  void handle_handshake(const boost::system::error_code& error);
  void handle_write(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_read_content(const boost::system::error_code& error);
  std::string buff_to_string (boost::asio::streambuf &MyBuffer);

public:
  std::string request_;
  std::string reply2;
  std::string header;
  std::string error_mess_;
  client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1);
};

#endif // A_REQUEST_H_INCLUDED
