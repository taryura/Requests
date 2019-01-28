#include "a_requests.h"
#include "head_type.h"

//For debug purposes
//#include "boost/lexical_cast.hpp"

client::client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1)
    : socket_(io_service, context){
  {
    request_ = rqst1;
    socket_.set_verify_mode(boost::asio::ssl::verify_none);
    socket_.set_verify_callback(
        boost::bind(&client::verify_certificate, this, _1, _2));

    boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
        boost::bind(&client::handle_connect, this,
          boost::asio::placeholders::error));
  }
}
  bool client::verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx){

    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

    return preverified;
  }

  void client::handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
          boost::bind(&client::handle_handshake, this,
            boost::asio::placeholders::error));
    }
    else
    {
      reply2 = ("Connect failed: " + error.message());
      std::cout << "Connect failed: " << error.message() << "\n";
    }
  }

  void client::handle_handshake(const boost::system::error_code& error)
  {
    if (!error)
    {


      size_t request_length = request_.length();


      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&client::handle_write, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      reply2 = ("Handshake failed: " + error.message());
      std::cout << "Handshake failed: " << error.message() << "\n";
    }
  }

  void client::handle_write(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      //reading header
      boost::asio::async_read_until(socket_,
          MyBuffer, "\r\n\r\n",
          boost::bind(&client::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      reply2 = ("Write failed: " + error.message());
      std::cout << "Write failed: " << error.message() << "\n";
    }
  }

void client::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      header = buff_to_string(MyBuffer);
      head_type parsed_header (header);
      //reading till EOF if content length is determined.
      std::string string1 = parsed_header.find_str("Content-Length");
      if (!parsed_header.err){
        std::string value1 = parsed_header.find_val(string1);

        //Retrieving "Content-Length" value
        int length = atoi(value1.c_str());

        //Length of the HTTP header
        int header_length = header.find ("\r\n\r\n");

        //Debug code
        //std::ostringstream s;
        //s << bytes_transferred;
        //error_mess_ = "\r\nBytes transfered: " + boost::lexical_cast<std::string>(bytes_transferred) +
        //"\r\n" + "Content-Length: " + value1 + "\r\nActual first part length: " + boost::lexical_cast<std::string>(header.length());
        //checking if everything has been already transfered


        //header.length = length of the first block,
        //header_length = Length of the HTTP header
        //length = Content length defined in HTTP header
        if ((header_length + 4 + length) > header.length()){
          //if not reading till EOF
          boost::asio::async_read(socket_, MyBuffer,
          boost::asio::transfer_at_least(length - (header.length() - header_length)),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
        }
        else {
            reply2 = header;
        }
      }
      else {
        //reading till EOF if chunked.
        string1 = parsed_header.find_str("Transfer-Encoding");
        if (!parsed_header.err){
          boost::asio::async_read_until(socket_,
            MyBuffer, "\r\n0\r\n",
            boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
        }
        else {
        //reading till EOF if HTML
        boost::asio::async_read_until(socket_,
            MyBuffer, "</html>",
            boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
      //reply2 = ("Read of the body failed: the length is undetermined. Please try to resend the request.\r\n" + header);
        }
      }

    }
    else
    {
      reply2 = ("Read failed: " + error.message());
      std::cout << "Read failed: " << error.message() << "\n";
    }
  }


void client::handle_read_content(const boost::system::error_code& error)
  {
    if (!error)
    {
      reply2 = header + buff_to_string(MyBuffer);
    }
    else
    {
      reply2 = ("Read2 failed: " + error.message()+ "\r\n" + header + error_mess_);
      std::cout << "Read failed: " << error.message() << "\n";
    }


  }

std::string client::buff_to_string (boost::asio::streambuf &MyBuffer)
{
      std::ostringstream BufOutStream;
      BufOutStream << &MyBuffer;
      return BufOutStream.str();
}






