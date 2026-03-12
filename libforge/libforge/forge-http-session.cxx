#include <libforge/forge-http-session.hxx>

#include <iostream>
#include <string>
#include <utility>

using namespace std;

namespace forge
{
  http_session::
  http_session (boost::asio::ip::tcp::socket s)
    : socket_ (std::move (s))
  {
  }

  void http_session::
  run ()
  {
    auto self (shared_from_this ());

    // Pin session to outlive asynchronous operations. Note that connection
    // can drop out from under us at any time.
    //
    boost::asio::co_spawn (socket_.get_executor (),
                           [self] ()
    {
      return self->process ();
    }, boost::asio::detached);
  }

  boost::asio::awaitable<void> http_session::
  process ()
  {
    try
    {
      for (;;)
      {
        boost::beast::http::request<boost::beast::http::string_body> q;

        // Read the next request. If the client closes connection, Beast
        // will signals end_of_stream.
        //
        co_await boost::beast::http::async_read (socket_,
                                                 flat_buffer_,
                                                 q,
                                                 boost::asio::use_awaitable);

        boost::beast::http::response<boost::beast::http::string_body> r (
          boost::beast::http::status::ok, q.version ());

        r.set (boost::beast::http::field::server, "forge-http");
        r.set (boost::beast::http::field::content_type, "text/plain");

        // Deduce if we should keep connection alive based on HTTP version
        // and Connection header.
        //
        bool k (q.keep_alive ());
        r.keep_alive (k);
        r.body ().assign ("Hello from forge-http\n");
        r.prepare_payload ();

        // Ship the response back.
        //
        co_await boost::beast::http::async_write (
          socket_, r, boost::asio::use_awaitable);

        if (!k)
          break;
      }
    }
    catch (const boost::system::system_error& e)
    {
      auto c (e.code ());

      // Normal for client to close connection or abort during shutdown.
      //
      if (c != boost::beast::http::error::end_of_stream &&
          c != boost::asio::error::operation_aborted)
      {
        println (cerr, "error: session failed: {}", c.message ());
      }

      co_return;
    }
    catch (const exception& e)
    {
      println (cerr, "error: unexpected exception during session process: {}",
               e.what ());
      co_return;
    }

    // Perform a shutdown of the send side of the socket.
    //
    try
    {
      if (socket_.is_open ())
        socket_.shutdown (boost::asio::ip::tcp::socket::shutdown_send);
    }
    catch (const boost::system::system_error& e)
    {
      auto c (e.code ());

      // Ignore errors if socket is already disconnected or reset by peer.
      //
      if (c != boost::asio::error::not_connected &&
          c != boost::asio::error::connection_reset)
      {
        println (cerr, "error: session shutdown failed: {}", c.message ());
      }
    }
    catch (const exception& e)
    {
      println (cerr, "error: unexpected exception during session shutdown: {}",
               e.what ());
    }
  }
}
