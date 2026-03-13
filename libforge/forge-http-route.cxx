#include <libforge/forge-http-route.hxx>

#include <cstdio>
#include <exception>
#include <print>
#include <string_view>
#include <utility>

namespace forge
{
  namespace
  {
    // Match the route path against the request path segment by segment. Extract
    // route parameters (like :id) along the way without copying strings.
    //
    bool
    match_path (std::string_view r, std::string_view p, forge::route_params& pr)
    {
      size_t rp (0);
      size_t pp (0);

      while (rp < r.size () && pp < p.size ())
      {
        size_t rn (r.find ('/', rp));
        size_t pn (p.find ('/', pp));

        std::string_view rs (r.substr (rp, rn - rp));
        std::string_view ps (p.substr (pp, pn - pp));

        // If the route segment starts with a colon, it is a parameter. Save it
        // as a view directly. Otherwise, the literal segments must match
        // perfectly.
        //
        if (!rs.empty () && rs[0] == ':')
          pr.emplace (rs.substr (1), ps);
        else if (rs != ps)
          return false;

        // See if we have reached the end of both paths.
        //
        if (rn == std::string_view::npos && pn == std::string_view::npos)
          return true;

        // If only one of the paths has ended, they do not match.
        //
        if (rn == std::string_view::npos || pn == std::string_view::npos)
          return false;

        rp = rn + 1;
        pp = pn + 1;
      }

      // Handle the edge case where both paths are completely empty or perfectly
      // matched up to the exact same length.
      //
      return r.size () == p.size ();
    }
  }

  void
  http_route::add (const verb m, std::string p, http_handler h)
  {
    rs_.push_back ({m, std::move (p), std::move (h)});
  }

  bool
  http_route::dispatch (const request<string_body>& q, response<string_body>& s)
  {
    auto t (q.target ());
    auto r (parse_uri_reference (t));

    if (!r)
    {
      // If the parser fails, the client sent a mangled request string. Treat
      // this as unroutable and bail out, letting the caller deal with the bad
      // request.
      //
      return false;
    }

    const auto& u (*r);
    auto m (q.method ());
    auto p (u.path ());

    // Iterate over our routing table to find a matching verb and path. Note
    // that we check the HTTP verb alongside the path. In REST, the exact same
    // path often carries entirely different semantics depending on the method.
    //
    for (auto& rt : rs_)
    {
      route_params pr;

      if (rt.m == m && match_path (rt.p, p, pr))
      {
        try
        {
          // We have a match. Invoke the handler and pass along the parsed URL
          // view and extracted parameters.
          //
          rt.h (q, s, u, pr);
        }
        catch (...)
        {
          s.result (internal_server_error);
          s.set (content_type, "text/plain");
          s.body ().assign ("Internal Server Error");
          s.prepare_payload ();
        }

        return true;
      }
    }

    // No route matched the incoming request.
    //
    return false;
  }
}
