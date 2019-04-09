#include "sslrequest.h"
#include "pop3client.h"

sslrequest::sslrequest (std::string address1, std::string port1)
{
    try
    {

        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(address1, port1);
        iterator = resolver.resolve(query);

    }

    catch (std::exception& e)
    {
    except_handler(e);
    }
}


void sslrequest::connect_set (std::string &requesttosend){

  try
  {

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    client c(io_service, ctx, iterator, requesttosend);

    io_service.run();

    replyreceived = c.reply2;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}

void sslrequest::connect_set (){

  try
  {

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    pop3client c(io_service, ctx, iterator);

    io_service.run();

    replyreceived = c.reply2;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}

void sslrequest::except_handler (std::exception& e)
{
    std::stringstream message;
    message << "Exception: ";
    message << e.what() << " ";
    message << "Please try again later\r\n";
    replyreceived = message.str();
    std::cerr << "Exception: Could not establish connection. Please try again later" << "\n";
}

