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

    //Initializing read handler function pointer
    handler_pointer = &pop3client::handle_read;

    //Initializing exchange buffer pointer
    threadBuff_ptr = stringstream_ptr;

    //mutex to block the exchange buffer
    mtx_ptr = mutex_ptr;

    request_ = "";
    reply2 = "";

    //reseting all flags
    authorized = FALSE;
    bufferReady = FALSE;
    requestInBuffer = FALSE;
    terminateIO = FALSE;
    IOterminated = FALSE;
  }
}

  void pop3client::handle_handshake(const boost::system::error_code& error)
  {

    if (!error)
    {

      //cancel timer
      tmr.cancel();


    tmr.expires_from_now(boost::posix_time::seconds(90));

    tmr.async_wait(boost::bind(&pop3client::shutdown, this, boost::asio::placeholders::error));

      //reading header
      boost::asio::async_read_until(socket_,
          MyBuffer, "\r\n",
          boost::bind(handler_pointer, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    }
    else
    {
      handle_error (error);
      reply2 = ("Write failed: " + error.message());
      std::cout << "Write failed: " << error.message() << "\n";
    }
  }

void pop3client::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      tmr.cancel();
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

      if (authorized)
      {
          handler_pointer = &pop3client::handle_read2;
      }

      if (request_ == "QUIT\r\n" || request_ == "quit\r\n")
      {
          handler_pointer = &pop3client::handle_read;
      }

      size_t request_length = request_.length();

    tmr.expires_from_now(boost::posix_time::seconds(90));

    tmr.async_wait(boost::bind(&pop3client::shutdown, this, boost::asio::placeholders::error));

      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&pop3client::handle_handshake, this,
            boost::asio::placeholders::error));


    }
    //if previous reading has ended with an error code
    else
    {
      handle_error (error);
      reply2 = "Read3 failed: " + error.message()+ "\r\n" + reply2;
      std::cout << "Read failed: " << error.message() << "\n";
    }

  }

void pop3client::handle_read2(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {

      tmr.cancel();
      mtx_ptr->lock();
      *threadBuff_ptr << &MyBuffer;
      mtx_ptr->unlock();

      request_ = "NOOP\r\n";
      size_t request_length = request_.length();


      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&pop3client::handle_write, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    }
    //if previous reading has ended with an error code
    else
    {
      handle_error (error);
      reply2 = "Read3 failed: " + error.message()+ "\r\n" + reply2;
      std::cout << "Read failed: " << error.message() << "\n";
    }

  }

void pop3client::handle_write(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {

      boost::asio::async_read_until(socket_,
          MyBuffer, "+OK",
          boost::bind(&pop3client::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    }
    //if previous reading has ended with an error code
    else
    {
      handle_error (error);
      reply2 = "Read3 failed: " + error.message()+ "\r\n" + reply2;
      std::cout << "Read failed: " << error.message() << "\n";
    }

  }

std::string pop3client::readBuffer (std::shared_ptr<std::stringstream> temp_threadBuff_ptr)
{
    std::string temp_string = temp_threadBuff_ptr->str();
    temp_threadBuff_ptr->str("");
    temp_threadBuff_ptr->clear();
    return temp_string;
}

void pop3client::shutdown (const boost::system::error_code& e2)
{
      if (e2){return;}

      boost::system::error_code eec;
      IOterminated = TRUE;
      throw std::logic_error("Connection lost\r\n");


      socket_.shutdown(eec);
      if (eec)
      {
          reply2 = "Error closing on timeout: " + eec.message();
      }
      else
      {
          reply2 = "Operation is canceled on timeout\r\n";
      }

}

void pop3client::handle_error (const boost::system::error_code& error)

{
      mtx_ptr->lock();
      *threadBuff_ptr << "\r\nError: " << error.message() << "\r\n";
      bufferReady = TRUE;
      IOterminated = TRUE;
      mtx_ptr->unlock();
}
