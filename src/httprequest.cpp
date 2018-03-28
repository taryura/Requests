#include "httprequest.h"

void httprequest::rqst_set (std::string addr, std::string prt, const std::string &req_text){

try
    {
    boost::asio::ip::tcp::iostream stream;

    //stream.expires_from_now(boost::posix_time::seconds(60));
    stream.connect(addr, "http");
    stream << req_text;
    stream << "Connection: close\r\n\r\n";
    stream.flush();
    std::stringstream request_2;
    request_2 << stream.rdbuf();
    replyreceived = request_2.str();
    }
catch (std::exception& e)
  {
    std::stringstream message;
    message << "Exception: ";
    message << e.what() << " ";
    message << "Please try again later";
    replyreceived = message.str();
    std::cerr << "Exception:" << e.what() << "\n";
  }

}
