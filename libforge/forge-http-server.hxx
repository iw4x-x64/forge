#pragma once

#include <libforge/forge.hxx>
#include <libforge/forge-http-route.hxx>

#include <libforge/export.hxx>

namespace forge
{
  class LIBFORGE_SYMEXPORT http_server
  {
  public:
    explicit
    http_server (port_type = 0);

    void
    run ();

    route_builder
    add_route (std::string p)
    {
      return route_builder (http_route_, std::move (p));
    }

  private:
    awaitable<void>
    accept ();

    awaitable<void>
    signals ();

  private:
    io_context io_;
    signal_set signals_;
    acceptor   acceptor_;
    http_route http_route_;
  };
}
