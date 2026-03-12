#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <libforge/forge-http-route.hxx>

namespace forge
{
  class http_session : public std::enable_shared_from_this<http_session>
  {
  public:
    explicit
    http_session (boost::asio::ip::tcp::socket, http_route&);

    void
    run ();

  private:
    boost::asio::awaitable<void>
    process ();

  private:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer flat_buffer_;
    http_route& router_;
  };
}
