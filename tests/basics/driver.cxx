#include <thread>

#include <libforge/forge-http-server.hxx>

#undef NDEBUG
#include <cassert>

int main ()
{
  using namespace std;
  using namespace forge;

  // Basics.
  //
  {
    http_server s (8080);

    jthread t ([] ()
    {
      this_thread::sleep_for (chrono::milliseconds (50));
      raise (SIGINT);
    });

    s.run ();
  }
}
