#include "a_requests.h"
//#include "head_type.h"
#include "parse_chunk.h"

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
      first_part = buff_to_string(MyBuffer);
      reply2 = first_part;
      //Length of the HTTP header
      header_length = first_part.find ("\r\n\r\n");

      //parsing header
      head_type parsed_header (first_part);


      chunk_to_transfer_int = parsed_header.chunk_bytes;

      //defines offset where the chunk starts
      cpi = 0;
      chunk_start_pointer[cpi] = header_length + 4 + parsed_header.chunk_bytes_to_read.length() + 2;

      //bytes to read = the value determined in the chunk HEX length - the length of the part we already received.
      next_transfer_length = chunk_to_transfer_int - (first_part.length() - chunk_start_pointer[cpi]);

      //Debug

      //reading till EOF if content length is determined.
      std::string string1 = parsed_header.find_str("Content-Length");
      if (!parsed_header.err){
        std::string value1 = parsed_header.find_val(string1);

        //Retrieving "Content-Length" value
        int length = atoi(value1.c_str());

        //checking if everything has been already transfered


        //header.length = length of the first block,
        //header_length = Length of the HTTP header
        //length = Content length defined in HTTP header
        if ((header_length + 4 + length) > first_part.length()){
          //if not reading till EOF
          boost::asio::async_read(socket_, MyBuffer,
          boost::asio::transfer_at_least(length - (first_part.length() - header_length)),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
        }
        else {
            reply2 = first_part;
        }
      }
      else {

        string1 = parsed_header.find_str("Transfer-Encoding");
        if (!parsed_header.err){

         //reading first chunk
          boost::asio::async_read(socket_, MyBuffer,
          boost::asio::transfer_at_least(next_transfer_length),
          boost::bind(&client::handle_read_chunk, this,
            boost::asio::placeholders::error));

        /*
        //reading till EOF if chunked.
          boost::asio::async_read_until(socket_,
            MyBuffer, "\r\n0\r\n",
            boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));*/
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

void client::handle_read_chunk(const boost::system::error_code& error)
  {
    if (!error)
    {
      //std::cout << reply2 << std::endl;

      reply2 += buff_to_string(MyBuffer);

      parse_chunk next_chunk_beginning (reply2.substr(chunk_start_pointer[cpi] + chunk_to_transfer_int));

      if (next_chunk_beginning.err)
      {
          boost::asio::async_read_until(socket_,
            MyBuffer, "\r\n",
            boost::bind(&client::handle_read_chunk, this,
            boost::asio::placeholders::error));
      }
      else
      {
          cpi +=1;
          chunk_start_pointer[cpi] = chunk_start_pointer[cpi-1] + chunk_to_transfer_int + 2 + next_chunk_beginning.chunk_bytes_to_read.length() + 2;
          next_transfer_length = chunk_to_transfer_int - (reply2.length() - chunk_start_pointer[cpi]);

          if (next_transfer_length > 0)
          {
          boost::asio::async_read(socket_, MyBuffer,
          boost::asio::transfer_at_least(next_transfer_length),
          boost::bind(&client::handle_read_chunk, this,
          boost::asio::placeholders::error));
          }
          else
          {
             reply2 = ("Something's wrong with chunked read: " + error.message()+ "\r\n" + reply2 + error_mess_);
          }


      }
    }
    else
    {
      reply2 = ("Read2 failed: " + error.message()+ "\r\n" + reply2 + error_mess_);
      std::cout << "Read failed: " << error.message() << "\n";
    }


  }


void client::handle_read_content(const boost::system::error_code& error)
  {
    if (!error)
    {
      reply2 = first_part + buff_to_string(MyBuffer);
    }
    else
    {
      reply2 = ("Read2 failed: " + error.message()+ "\r\n" + first_part + error_mess_);
      std::cout << "Read failed: " << error.message() << "\n";
    }


  }

std::string client::buff_to_string (boost::asio::streambuf &MyBuffer)
{
      std::ostringstream BufOutStream;
      BufOutStream << &MyBuffer;
      return BufOutStream.str();
}






