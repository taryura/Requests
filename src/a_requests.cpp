#include "a_requests.h"

//For debug purposes
#include "boost/lexical_cast.hpp"

client::client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1)
      : clientBase(io_service, context, endpoint_iterator){
  {
    request_ = rqst1;
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
       handle_error (error);
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
       handle_error (error);
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
      std::cout << "Reply2 after header read:\r\n\r\n " << reply2 << std::endl;

      header_length = first_part.find ("\r\n\r\n");

      //parsing header
      parse_head parsed_header (first_part);

      //reading till EOF if content length is determined.
      std::string string1 = parsed_header.find_str("Content-Length");

      if (!parsed_header.err)
        {
        std::string value1 = parsed_header.find_val(string1);

        //Retrieving "Content-Length" value
        int length = atoi(value1.c_str());

        //checking if everything has been already transfered


        //header.length = length of the first block,
        //header_length = Length of the HTTP header
        //length = Content length defined in HTTP header

        int fpl = first_part.length();
        const int crlf_length = 4;
        if ((header_length + crlf_length + length) > fpl)
          {
          //if not reading till EOF
          boost::asio::async_read(socket_, MyBuffer,
          boost::asio::transfer_at_least(length - (first_part.length() - header_length)),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
          }
        else
        {
            reply2 = first_part;
        }
      }
      else
        {

        string1 = parsed_header.find_str("Transfer-Encoding");
        if (!parsed_header.err){

               //chunk_to_transfer_int = parsed_header.chunk_bytes;

          //defines offset where the chunk starts
          cpi = 0;
          chunk_start_pointer[cpi] = 0;
          const int crlf_length = 2;
          chunk_length[cpi] = header_length + crlf_length;

          //Delegating to chunk processing method
          std::cout << "Reading next chunk after the header:\r\n\r\n " << std::endl;
          handle_read_chunk (error);

        }
        else
        {
        //reading till EOF if neither the content-length
        //nor the chunked transfer defined (till HTML close tag)

        boost::asio::async_read_until(socket_,
            MyBuffer, "</html>",
            boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
        }
       }

    }
    //if previous reading has ended with an error code
    else
    {
       handle_error (error);
    }
  }

void client::handle_read_chunk(const boost::system::error_code& error)
  {
    if (!error)
    {
      reply2 += buff_to_string(MyBuffer);
      std::cout << "Reply2:\r\n\r\n " << reply2 << std::endl;

      //parsing the chunk received
  try
     {

      parse_chunk next_chunk_beginning (reply2, chunk_start_pointer, chunk_length, cpi);


      if (next_chunk_beginning.end_of_file)
      {
              std::cout << "End of file:\r\n\r\n " << reply2 << std::endl;
              return;
      }

      else
      {

        //Reading more data if the length field is not fully transfered
        if (next_chunk_beginning.err or next_chunk_beginning.end_of_chunk)
        {
            std::cout << "Reading till next clrf" << std::endl;

            boost::asio::async_read_until(socket_,
              MyBuffer, "\r\n",
              boost::bind(&client::handle_read_chunk, this,
              boost::asio::placeholders::error));

        }

        //Reading next chunk if its length is determined
        else
        {
            std::cout << "Reading next chunk: " << next_chunk_beginning.next_transfer_length << "Bytes" << std::endl;
            boost::asio::async_read(socket_, MyBuffer,
            boost::asio::transfer_at_least(next_chunk_beginning.next_transfer_length),
            boost::bind(&client::handle_read_chunk, this,
            boost::asio::placeholders::error));
        }

      }

     }
         //any exception during parsing
         catch (...)
      {

      reply2 = reply2
      + "\r\n\r\n"
      + "\r\n Debug " + "CHUNK_START_POINTER: "
      + boost::lexical_cast<std::string>(chunk_start_pointer[cpi])+ "\r\n"
      + "CH_LEN " + boost::lexical_cast<std::string>(chunk_length[cpi]) + "\r\n"
      + "CPI= " + boost::lexical_cast<std::string>(cpi) + "\r\n";
      //+ "Next transfer length: " + boost::lexical_cast<std::string>(next_chunk_beginning.next_transfer_length) + "\r\n";
      return;
      }
    }
    //if previous reading has ended with an error code
    else
    {
       handle_error (error);
    }

  }

void client::handle_read_content(const boost::system::error_code& error)
  {
    if (!error)
    {
      reply2 = first_part + buff_to_string(MyBuffer);
    }

    //if previous reading has ended with an error code
    else
    {
       handle_error (error);
    }


  }

std::string client::buff_to_string (boost::asio::streambuf &MyBuffer)
{
      std::ostringstream BufOutStream;
      BufOutStream << &MyBuffer;
      return BufOutStream.str();
}

void client::handle_error (const boost::system::error_code& error)

{

      reply2 = "\r\nError: " + error.message() + "\r\n";
      std::cout << reply2;
}






