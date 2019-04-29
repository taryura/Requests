#include "clientBase.h"

clientBase::clientBase(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
           : socket_(io_service, context), tmr(io_service){

    socket_.set_verify_mode(boost::asio::ssl::verify_none);
    socket_.set_verify_callback(
        boost::bind(&clientBase::verify_certificate, this, _1, _2));

    boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
        boost::bind(&clientBase::handle_connect, this,
          boost::asio::placeholders::error));

}
  bool clientBase::verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx){

    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

    return preverified;
  }

  void clientBase::handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
          boost::bind(&clientBase::handle_handshake, this,
            boost::asio::placeholders::error));
    }
    else
    {
       handle_error (error);
    }
  }

//virtual void clientBase::handle_handshake(const boost::system::error_code& error) =0;
