# Forge

`Forge` is a C++ HTTP server library built on `boost::beast` and `boost::asio` coroutines, with handler binding driven by OpenAPI specifications.

## Getting Started

Probably the first thing you will want to do with `Forge` is get a basic HTTP server up and running. Let's look at a minimal application that does exactly that:

```cpp
#include <libforge/forge-http-server.hxx>

#include <exception>
#include <iostream>
#include <print>

using namespace std;

int
main ()
{
  try
  {
    forge::http_server s (8080);
    s.run ();
  }
  catch (const exception& e)
  {
    println (cerr, "error: {}", e.what ());
    return 1;
  }
}

```

The first half of our `main()` function includes the required headers and wraps the initialization in a `try-catch` block. If a fatal error occurs, for instance if the requested port is already in use, the server throws a standard exception.

Passing `8080` to the constructor tells the server to bind its TCP acceptor to this specific port. If we pass nothing instead, the operating system picks a random available port for us. Under the hood, the constructor also registers handlers for `SIGINT` and `SIGTERM`. This way, we can stop the server later on by pressing `Ctrl+C`.

With the server object ready, we then start the execution loop by calling `run ()`:

```cpp
s.run ();

```

This call blocks the current thread and starts processing I/O events. At this point, the server just stays in this loop, accepting incoming connections in the background, until it catches an OS termination signal or a fatal network error forces it to bail out.

### OpenAPI

While getting a server to listen on a port is a great start, it doesn't do much without any endpoints to handle incoming requests. Describing Forge's design purely as an OpenAPI-first approach to *routing* is not quite right, since the OpenAPI Specification (OAS) defines a standard, programming language-agnostic interface description for HTTP APIs.

Instead, Forge embraces an *interface-first* architecture. Rather than manually wiring up HTTP verbs and paths in your C++ code, you define your API contract using an OAS document (JSON) and bind your C++ handlers directly to the `operationId`s defined within that language-agnostic interface.

Let's expand our minimal application to include a simple hello world endpoint:

```cpp
#include <libforge/forge-http-server.hxx>

#include <exception>
#include <iostream>
#include <print>
#include <string_view>

using namespace std;

const std::string_view api_spec = R"({
  "openapi": "3.2.0",
  "info": {
    "title": "My Forge API",
    "version": "1.0.0"
  },
  "paths": {
    "/v1/hello": {
      "get": {
        "operationId": "hello",
        "summary": "Hello world endpoint"
      }
    }
  }
})";

int
main ()
{
  try
  {
    forge::http_server s (8080);

    s.openapi (api_spec);

    s.router ().load_spec (api_spec);
    s.router ().handle ("hello", [] (const auto& req, auto& res)
    {
      res.result (boost::beast::http::status::ok);
      res.set (boost::beast::http::field::content_type, "application/json");
      res.body ().assign (R"({"message": "hello world"})");
      res.prepare_payload ();
    });

    s.run ();
  }
  catch (const exception& e)
  {
    println (cerr, "error: {}", e.what ());
    return 1;
  }
}

```

First we define our API contract using standard OpenAPI v3 JSON. While you would typically loads this from an external file, we use a string literal here for the example.

Next, we configure the server using three distinct method calls. Calling `s.openapi (api_spec)` automatically mounts an endpoint, usually `/openapi.json`, to serve your specification directly. After that, `s.router ().load_spec (api_spec)` tells Forge to parse the JSON specification and internally map out the required HTTP methods and URL paths. Finally, we bind our lambda directly to the `operationId` defined in the OpenAPI spec using `s.router ().handle ("hello", ...)`, rather than manually mapping to `/v1/hello` and the `GET` method.

Notice that the request and response objects expose standard Boost.Beast and Boost.HTTP interfaces. This grants you full access to the underlying Beast primitives.
