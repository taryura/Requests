#include "ssl_connect.h"
#include <chrono>

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
            try
            {
            client_run (threadBuff_ptr, mtx_ptr);
            }
            catch (std::logic_error &ec)
            {
                replyreceived = ec.what();
                throw;
            }

        });

    if (t1.joinable())
        {
            t1.detach();
        }

    //waiting the thread to create the client
    while (!c2)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }

    //waiting the thread to fill the buffer
    while (!c2->bufferReady)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
    mtx_ptr->lock();
    replyreceived = pop3client::readBuffer(threadBuff_ptr);
    c2->bufferReady = FALSE;
    mtx_ptr->unlock();

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

    if (c2->IOterminated)
    {
        replyreceived = "\r\n\r\nIO terminated";
        return;
    }

    mtx_ptr->lock();
    *threadBuff_ptr << requesttosend;
    mtx_ptr->unlock();
    c2->requestInBuffer = TRUE;


    while (!c2->bufferReady)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
    mtx_ptr->lock();
    replyreceived = pop3client::readBuffer(threadBuff_ptr);
    c2->bufferReady = FALSE;
    mtx_ptr->unlock();

    if (requesttosend == "QUIT\r\n" || requesttosend == "quit\r\n")
    {
       authorized = false;
       stopclient();
    }

    //pop3 command length
    if (!authorized)
    {
    unsigned int pop3comlength = 4;
    if (requesttosend.length() >= pop3comlength)
    {
        if ((requesttosend.substr(0, pop3comlength) == "PASS") || (requesttosend.substr(0, pop3comlength) == "pass"))
        {
            //pop3 OK respond code length
            unsigned int pop3Oklength = 3;
            if (replyreceived.length() >= pop3Oklength)
            {

                //replyreceived += ("\r\n" + replyreceived.substr(0, pop3Oklength)+ "\r\n");
                if (replyreceived.substr(0, pop3Oklength) == "+OK")
                    {
                        c2->authorized = TRUE;
                        authorized = TRUE;
                    }
                else
                    {
                        replyreceived += "\r\nNot authorized";
                        stopclient();
                    }
            }


        }
    }

    }

    else
    {
        for (int i = replyreceived.length(); i > 3; i --)

            //We need to find the last "+OK" so 75 = "K" 79 = "O" and 43 = "+"
        if (replyreceived[i] == 75 && replyreceived[i-1] == 79 && replyreceived[i-2] == 43)
        {

            //Cutting out the last "+OK" from the response
            replyreceived = replyreceived.substr(0, (i - 3));
            //once the last "+OK" is found we stop the loop
            break;
        }

    }


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

void ssl_connect::stopclient()
{
       replyreceived += "\r\n\r\nConnection closed";
       c2->terminateIO = TRUE;
       while (!c2->IOterminated)
        {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        }
}
