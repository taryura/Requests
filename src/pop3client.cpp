#include "pop3client.h"

pop3client::pop3client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
      : clientBase(io_service, context, endpoint_iterator){
  {
    request_ = "QUIT";
  }
}

void pop3client::handle_handshake(const boost::system::error_code& error)
  {
    if (!error)
    {
      //reading header
      size_t request_length = request_.length();


      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&pop3client::handle_read2, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      reply2 = ("Write failed: " + error.message());
      std::cout << "Write failed: " << error.message() << "\n";
    }
  }

void pop3client::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      reply2 = buff_to_string(MyBuffer);
      size_t request_length = request_.length();
        boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&pop3client::handle_read2, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    //if previous reading has ended with an error code
    else
    {
      reply2 = ("Read failed: " + error.message());
      std::cout << "Read failed: " << error.message() << "\n";
    }
  }

void pop3client::handle_read2(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      reply2 += buff_to_string(MyBuffer);
      boost::asio::async_read_until(socket_,
          MyBuffer, "\r\n",
          boost::bind(&pop3client::handle_read2, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    //if previous reading has ended with an error code
    else
    {
      reply2 += ("Read failed: " + error.message());
      std::cout << "Read failed: " << error.message() << "\n";
    }
  }



void pop3client::write_req(std::string &request_to_write)
  {

      size_t request_length = request_to_write.length();
      try
      {
        boost::asio::async_write(socket_,
          boost::asio::buffer(request_to_write.c_str(), request_length),
          boost::bind(&pop3client::handle_write, this,
            boost::asio::placeholders::error));

            reply2 = "is it ignoring me?";
      }
      catch (...)
      {
          reply2 = "Error write";
      }


  }

void pop3client::handle_write(const boost::system::error_code& error)
{
      if (!error)
    {

        reply2 = "Handle write";
    }
    else
    {
      reply2 = ("Read failed: " + error.message());
      std::cout << "Read failed: " << error.message() << "\n";
    }
}



std::string pop3client::buff_to_string (boost::asio::streambuf &MyBuffer)
{
      std::ostringstream BufOutStream;
      BufOutStream << &MyBuffer;
      return BufOutStream.str();
}
