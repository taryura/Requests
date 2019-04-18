#include <thread>
#include <chrono>
#include "pop3client.h"


pop3client::pop3client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::shared_ptr<std::stringstream> stringstream_ptr,
      std::shared_ptr<std::mutex> mutex_ptr)
      : clientBase(io_service, context, endpoint_iterator){
  {
    threadBuff_ptr = stringstream_ptr;
    mtx_ptr = mutex_ptr;

    request_ = "";
    reply2 = "";
    bufferReady = FALSE;
    requestInBuffer = FALSE;
    terminateIO = FALSE;
    IOterminated = FALSE;
  }
}

  void pop3client::handle_handshake(const boost::system::error_code& error)
  {
    size_t request_length = request_.length();

    if (!error)
    {
      //reading header
      boost::asio::async_read_until(socket_,
          MyBuffer, "\r\n",
          boost::bind(&pop3client::handle_read, this,
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
      mtx_ptr->lock();
      *threadBuff_ptr << &MyBuffer;
      bufferReady = TRUE;
      mtx_ptr->unlock();


      //waiting for the request from the main thread
      while (!requestInBuffer)
      {
        if (terminateIO)
          {
           IOterminated = TRUE;
           return;
          }
          std::this_thread::sleep_for (std::chrono::milliseconds(100));
      }
      mtx_ptr->lock();
      request_ = readBuffer(threadBuff_ptr);
      requestInBuffer = FALSE;
      mtx_ptr->unlock();

      size_t request_length = request_.length();


      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&pop3client::handle_handshake, this,
            boost::asio::placeholders::error));

    }
    //if previous reading has ended with an error code
    else
    {
      reply2 = "Read3 failed: " + error.message()+ "\r\n" + reply2;
      std::cout << "Read failed: " << error.message() << "\n";
    }

  }

std::string pop3client::buff_to_string (boost::asio::streambuf &MyBuffer)
{
      std::stringstream BufOutStream;
      BufOutStream << &MyBuffer;
      return BufOutStream.str();
}

std::string pop3client::readBuffer (std::shared_ptr<std::stringstream> temp_threadBuff_ptr)
{
    std::string temp_string = temp_threadBuff_ptr->str();
    temp_threadBuff_ptr->str("");
    temp_threadBuff_ptr->clear();
    return temp_string;
}
