#ifndef A_REQUESTS_H_INCLUDED
#define A_REQUESTS_H_INCLUDED

#include <cstring>
#include <string>
#include "clientBase.h"
#include "parse_chunk.h"
#include "parse_head.h"
#include "input_validator.h"

class client : public clientBase{
private:
  boost::asio::streambuf MyBuffer;
  void handle_handshake(const boost::system::error_code& error);
  void handle_write(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_read_content(const boost::system::error_code& error);
  void handle_read_chunk(const boost::system::error_code& error);
  std::string buff_to_string (boost::asio::streambuf &MyBuffer);

public:
  std::string first_part;
  std::string error_mess_;
  int header_length, chunk_to_transfer_int, chunk_start_pointer[128], chunk_length[128], cpi;
  client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1);
};

#endif // A_REQUEST_H_INCLUDED
