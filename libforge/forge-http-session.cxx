#include <libforge/forge-http-session.hxx>

#include <iostream>
#include <string>
#include <utility>
#include <print>

namespace forge
{
  http_session::
  http_session (socket s, http_route& r)
    : socket_ (std::move (s)),
      http_route_ (r)
  {
  }

  void http_session::
  run ()
  {
    auto self (shared_from_this ());

    // Pin the session to outlive asynchronous operations. Note that the
    // connection can drop out from under us at any time, which we expect
    // and handle down below.
    //
    co_spawn (socket_.get_executor (),
              [self] ()
    {
      return self->process ();
    }, detached);
  }

  awaitable<void> http_session::
  process ()
  {
    try
    {
      for (;;)
      {
        request<string_body> rq;

        // Read the next request. If the client closes the connection,
        // Beast signals end_of_stream.
        //
        co_await async_read (socket_, flat_buffer_, rq, use_awaitable);

        response<string_body> rs (ok, rq.version ());

        rs.set (server, "forge-http");
        rs.set (content_type, "text/plain");

        // Deduce if we should keep the connection alive based on the HTTP
        // version and Connection header before consuming the request.
        //
        bool k (rq.keep_alive ());

        // Attempt to dispatch the request to the appropriate route handler.
        // If none matches, deduce whether it's a bad URI or a simple 404.
        //
        bool d (http_route_.dispatch (rq, rs));

        if (!d)
        {
          auto t (rq.target ());

          if (!parse_uri_reference (t))
          {
            rs.result (bad_request);
            rs.body ().assign ("Bad Request\n");
          }
          else
          {
            rs.result (not_found);
            rs.body ().assign ("Not Found\n");
          }
        }

        rs.prepare_payload ();

        // Ship the response back to the client.
        //
        co_await async_write (socket_, rs, use_awaitable);

        if (!k)
          break;
      }
    }
    catch (const system_error& e)
    {
      auto c (e.code ());

      // It is perfectly normal for the client to close the connection or
      // for the read/write to abort during a server shutdown.
      //
      if (c != end_of_stream && c != operation_aborted)
        std::println (std::cerr, "error: session failed: {}", c.message ());

      co_return;
    }
    catch (const std::exception& e)
    {
      std::println (std::cerr, "error: unexpected exception: {}", e.what ());
      co_return;
    }

    // Perform a graceful shutdown of the send side of the socket.
    //
    try
    {
      if (socket_.is_open ())
        socket_.shutdown (shutdown_send);
    }
    catch (const system_error& e)
    {
      auto c (e.code ());

      // Ignore errors if the socket is already disconnected or was reset
      // by the peer.
      //
      if (c != not_connected && c != connection_reset)
        std::println (std::cerr, "error: session shutdown failed: {}", c.message ());
    }
    catch (const std::exception& e)
    {
      std::println (std::cerr, "error: unexpected shutdown exception: {}", e.what ());
    }
  }
}
