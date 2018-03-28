#include "sslrequest.h"

void sslrequest::rqst_set (std::string address1,std::string port1,std::string &requesttosend){

  try
  {
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(address1, port1);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);

    client c(io_service, ctx, iterator, requesttosend);

    io_service.run();

    replyreceived = c.reply2;
  }
  catch (std::exception& e)
  {
    std::stringstream message;
    message << "Exception: ";
    message << e.what() << " ";
    message << "Please try again later";
    replyreceived = message.str();
    std::cerr << "Exception: Could not establish connection. Please try again later" << "\n";

  }

}
