#include "a_requests.h"

client::client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
      std::string rqst1)
    : socket_(io_service, context){
  {
    request_ = rqst1;
    socket_.set_verify_mode(boost::asio::ssl::verify_none);
    socket_.set_verify_callback(
        boost::bind(&client::verify_certificate, this, _1, _2));

    boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
        boost::bind(&client::handle_connect, this,
          boost::asio::placeholders::error));
  }
}
  bool client::verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx){

    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

    return preverified;
  }

  void client::handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
          boost::bind(&client::handle_handshake, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Connect failed: " << error.message() << "\n";
    }
  }

  void client::handle_handshake(const boost::system::error_code& error)
  {
    if (!error)
    {


      size_t request_length = request_.length();


      boost::asio::async_write(socket_,
          boost::asio::buffer(request_.c_str(), request_length),
          boost::bind(&client::handle_write, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cout << "Handshake failed: " << error.message() << "\n";
    }
  }

  void client::handle_write(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {

      boost::asio::async_read_until(socket_,
          MyBuffer, 'H',
          boost::bind(&client::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cout << "Write failed: " << error.message() << "\n";
    }
  }

  void client::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {

      std::ostringstream BufOutStream;
      BufOutStream << &MyBuffer;
      reply2 = BufOutStream.str();

    }
    else
    {
      std::cout << "Read failed: " << error.message() << "\n";
    }
  }


void sslrequest::rqst_set (std::string addr,std::string prt,std::string &req_text){
            requesttosend = req_text;
            address1 = addr;
            port1 = prt;
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
    std::cerr << "Exception: Could not establish connection. Please try again later" << "\n";

  }

}


