# Forge

## Getting Started

Probably the first thing you will want to do with Forge is get a basic HTTP server up and running. Let's look at a minimal application that does exactly that:

```cpp
#include <libforge/forge-http-server.hxx>

#include <iostream>
#include <print>
#include <exception>

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

