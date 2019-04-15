#include "sslrequest.h"

// ssl connecor


sslrequest::sslrequest (std::string address1, std::string port1, std::string &requesttosend)
                              : ssl_base (address1, port1)
{

  try
  {

    boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);
    client c(io_service, ctx, iterator, requesttosend);

    io_service.run();

    replyreceived = c.reply2;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}




