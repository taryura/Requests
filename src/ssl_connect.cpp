#include "ssl_connect.h"

ssl_connect::ssl_connect(std::string address1, std::string port1)
                         : ssl_base (address1, port1)
{

connected = false;
authorized = false;

      try
  {

    //creating a mutex for shared stringstream
    std::shared_ptr<std::mutex> temp_mtx(new std::mutex);
    mtx_ptr = temp_mtx;

    //creating a new shared stringstream
    std::shared_ptr<std::stringstream> temp_buff(new std::stringstream);
    threadBuff_ptr = temp_buff;


    t1 = std::thread([this]()
        {
            client_run (threadBuff_ptr, mtx_ptr);
        });

    if (t1.joinable())
        {
            t1.detach();
        }

    //waiting the thread to create the client
    while (!c2)
    {

    }

    //waiting the thread to fill the buffer
    while (!c2->bufferReady)
    {

    }

    replyreceived = readBuffer();
    connected = true;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}

void ssl_connect::connect_send (std::string &requesttosend){

  try
  {

    //c2->write_req(requesttosend);
    //replyreceived = c2->reply2;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}

void ssl_connect::client_run (std::shared_ptr<std::stringstream> stringstream_ptr,
                              std::shared_ptr<std::mutex> mutex_ptr)
{
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv3_client);

    std::unique_ptr<pop3client> c(new pop3client(io_service, ctx, iterator,
                                                 stringstream_ptr, mutex_ptr));
    c.swap(c2);

    io_service.run();
}

std::string ssl_connect::readBuffer ()
{
    mtx_ptr->lock();
    std::string temp_string = threadBuff_ptr->str();
    threadBuff_ptr->str("");
    threadBuff_ptr->clear();
    mtx_ptr->unlock();
    return temp_string;
}
