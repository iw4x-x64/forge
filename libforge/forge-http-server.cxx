#include <libforge/forge-http-server.hxx>
#include <libforge/forge-http-session.hxx>

#include <iostream>

using namespace std;

namespace forge
{
  http_server::
  http_server (port_type p)
    : http_route_ (),
      io_ (),
      signals_ (io_, SIGINT, SIGTERM),
      acceptor_ (io_)
  {
    endpoint ep (v4 (), p);

    acceptor_.open (ep.protocol ());
    acceptor_.set_option (reuse_address (true));
    acceptor_.bind (ep);
    acceptor_.listen (max_listen_connections);
  }

  void http_server::
  run ()
  {
    // Spawn our background coroutines. Note that their lifetime is managed
    // entirely by the io_context execution loop.
    //
    co_spawn (io_,  accept (), detached);
    co_spawn (io_, signals (), detached);

    // Block until io_.stop() is invoked, either by our signal handler
    // or by an unrecoverable exception propagating from the accept loop.
    //
    io_.run ();
  }

  awaitable<void> http_server::
  accept ()
  {
    for (;;)
    {
      try
      {
        auto s (co_await acceptor_.async_accept (use_awaitable));

        // Create a session to manage the lifetime of the accepted connection
        // and transfer ownership of the socket to it.
        //
        auto session (
          std::make_shared<http_session> (std::move (s), http_route_));
        session->run ();
      }
      catch (const system_error& e)
      {
        auto c (e.code ());

        if (c == operation_aborted)
          break;

        // Handle actual accept errors. Note that a common mistake is to quietly
        // swallow resource exhaustion errors (like EMFILE). If we do that, the
        // io_context will instantly wake up and we will end up spinning in a
        // tight CPU-burning loop. Log the message and stop the server to
        // properly propagate the failure out of run ().
        //
        println (cerr, "error: accept failed: {}", e.what ());
        break;
      }
      catch (const std::exception& e)
      {
        // Catch any other standard exceptions and bail.
        //
        println (cerr, "error: unexpected exception in accept: {}", e.what ());
        break;
      }
    }

    io_.stop ();
  }

  awaitable<void> http_server::
  signals ()
  {
    try
    {
      // Suspend until we receive SIGINT or SIGTERM from the OS.
      //
      co_await signals_.async_wait (use_awaitable);

      io_.stop ();
    }
    catch (system_error& e)
    {
      auto c (e.code ());

      // If the io_context is stopped from the accept loop due to a fatal
      // error, we will get an operation_aborted error here. We can safely
      // ignore it, but anything else indicates a deeper underlying issue.
      //
      if (c != operation_aborted)
        println (cerr, "error: signal wait failed: {}", e.what ());
    }
  }
}
