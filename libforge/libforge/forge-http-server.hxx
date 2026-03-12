#pragma once

#include <boost/asio.hpp>

#include <libforge/forge-http-route.hxx>

namespace forge
{
  class http_server
  {
  public:
    explicit
    http_server (boost::asio::ip::port_type = 0);

    void
    run ();

    route_builder
    add_route (std::string p)
    {
      return route_builder (router_, std::move (p));
    }

  private:
    boost::asio::awaitable<void>
    accept ();

    boost::asio::awaitable<void>
    signals ();

  private:
    boost::asio::io_context io_;
    boost::asio::signal_set signals_;
    boost::asio::ip::tcp::acceptor acceptor_;

    http_route router_;
  };
}
