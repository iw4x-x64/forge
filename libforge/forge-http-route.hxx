#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <libforge/forge.hxx>

namespace forge
{
  using route_params = std::map<std::string, std::string, std::less<>>;
  using http_handler =
    std::move_only_function<void (const request<string_body>&,
                                  response<string_body>&,
                                  const boost::urls::url_view&,
                                  const route_params&)>;

  class http_route;

  class route_builder
  {
  public:
    route_builder (http_route& r, std::string p)
      : r_ (r),
        p_ (std::move (p))
    {
    }

    template <typename H>
    route_builder&
    get (H&& h)
    {
      return add (verb::get, std::forward<H> (h));
    }

    template <typename H>
    route_builder&
    post (H&& h)
    {
      return add (verb::post, std::forward<H> (h));
    }

    template <typename H>
    route_builder&
    put (H&& h)
    {
      return add (verb::put, std::forward<H> (h));
    }

    template <typename H>
    route_builder&
    patch (H&& h)
    {
      return add (verb::patch, std::forward<H> (h));
    }

    template <typename H>
    route_builder&
    del (H&& h)
    {
      return add (verb::delete_, std::forward<H> (h));
    }

    template <typename H>
    route_builder&
    add (verb m, H&& h);

  private:
    http_route& r_;
    std::string p_;
  };

  class http_route
  {
  public:
    void
    add (verb, std::string, http_handler);

    bool
    dispatch (const request<string_body>&, response<string_body>&);

  private:
    struct route
    {
      verb m;
      std::string p;
      http_handler h;
    };

    std::vector<route> rs_;
  };

  template <typename H>
  route_builder&
  route_builder::add (verb m, H&& h)
  {
    // We wrap the user-provided handler in a generic lambda that accepts the
    // absolute maximum set of arguments our routing framework can produce and
    // then forward the user's handler into the lambda's capture block.
    //
    r_.add (m,
            p_,
            [f = std::forward<H> (h)] (const request<string_body>& q,
                                       response<string_body>& s,
                                       const boost::urls::url_view& u,
                                       const route_params& pr) mutable
    {
      // Now we simply ask the compiler: can this callable be invoked with
      // all four arguments? If so, we instantiate this branch and the
      // compiler throws away the rest.
      //
      if constexpr (std::is_invocable_v<decltype (f)&,
                                        const request<string_body>&,
                                        response<string_body>&,
                                        const boost::urls::url_view&,
                                        const route_params&>)
      {
        f (q, s, u, pr);
      }
      // If not four, how about three? This signature skips the raw url_view
      // but keeps the parsed route_params.
      //
      else if constexpr (std::is_invocable_v<decltype (f)&,
                                             const request<string_body>&,
                                             response<string_body>&,
                                             const route_params&>)
      {
        f (q, s, pr);
      }
      // Finally, the simplest case: just the request and response objects.
      //
      else if constexpr (std::is_invocable_v<decltype (f)&,
                                             const request<string_body>&,
                                             response<string_body>&>)
      {
        f (q, s);
      }
      // If the callable does not match any of our supported signatures, we
      // hit this fallback.
      //
      else
      {
        static_assert (false,
          "Handler signature must be: (const request<string_body>&, "
          "response<string_body>&, [const url_view&], [const route_params&])"
        );
      }
    });

    return *this;
  }
}
