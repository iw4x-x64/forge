#pragma once

#include <boost/asio.hpp>

#include <boost/beast.hpp>

#include <boost/url.hpp>

namespace forge
{
  // Core asynchronous and coroutine primitives.
  //
  using boost::asio::awaitable;
  using boost::asio::co_spawn;
  using boost::asio::detached;
  using boost::asio::use_awaitable;

  // Networking primitives.
  //
  using boost::asio::ip::tcp;
  using boost::asio::socket_base;

  using port_type = boost::asio::ip::port_type;
  using socket = tcp::socket;
  using endpoint = tcp::endpoint;
  using reuse_address = socket_base::reuse_address;

  // Socket operations and expected error codes.
  //
  using boost::asio::error::connection_reset;
  using boost::asio::error::not_connected;
  using boost::asio::error::operation_aborted;

  // Map class member constants into our namespace.
  //
  constexpr auto shutdown_send (socket_base::shutdown_send);
  constexpr auto max_listen_connections (socket_base::max_listen_connections);

  // Beast HTTP server abstractions.
  //
  using boost::beast::flat_buffer;

  using boost::beast::http::async_read;
  using boost::beast::http::async_write;
  using boost::beast::http::request;
  using boost::beast::http::response;
  using boost::beast::http::string_body;
  using boost::beast::http::verb;

  // HTTP header fields.
  //
  using boost::beast::http::field::content_type;
  using boost::beast::http::field::server;

  // HTTP status codes.
  //
  using boost::beast::http::status::bad_request;
  using boost::beast::http::status::internal_server_error;
  using boost::beast::http::status::not_found;
  using boost::beast::http::status::ok;

  // Beast specific error codes.
  //
  using boost::beast::http::error::end_of_stream;

  // System diagnostics.
  //
  using boost::system::system_error;

  // URL parsing components.
  //
  using boost::urls::parse_uri_reference;
}
