#include "ssl_connect.h"

ssl_connect::ssl_connect(std::string address1, std::string port1)
                         : ssl_base (address1, port1)
{

connected = false;
authorized = false;

      try
  {

    //boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    std::unique_ptr<pop3client> c(new pop3client(io_service, ctx, iterator));
    c.swap(c2);

    io_service.run();

    replyreceived = c2->reply2;
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

    c2->write_req(requesttosend);
    replyreceived = c2->reply2;
  }
  catch (std::exception& e)
  {
    except_handler(e);
  }

}
