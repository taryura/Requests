#include "ssl_base.h"


ssl_base::ssl_base (std::string address1, std::string port1)
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


void ssl_base::except_handler (std::exception& e)
{
    std::stringstream message;
    message << "Exception: ";
    message << e.what() << " ";
    message << "Please try again later\r\n";
    replyreceived = message.str();
    std::cerr << "Exception: Could not establish connection. Please try again later" << "\n";
}
