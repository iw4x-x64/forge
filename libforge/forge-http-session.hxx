#pragma once

#include <libforge/forge.hxx>
#include <libforge/forge-http-route.hxx>

#include <libforge/export.hxx>

namespace forge
{
  class LIBFORGE_SYMEXPORT http_session
    : public std::enable_shared_from_this<http_session>
  {
  public:
    explicit
    http_session (socket, http_route&);

    void
    run ();

  private:
    awaitable<void>
    process ();

  private:
    socket      socket_;
    flat_buffer flat_buffer_;
    http_route& http_route_;
  };
}
