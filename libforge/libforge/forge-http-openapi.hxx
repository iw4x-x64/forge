#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <boost/json.hpp>

namespace forge
{
  struct schema;
  struct media_type;
  struct path_item;
  struct encoding;

  struct reference
  {
    std::string $ref;
    std::optional<std::string> summary;
    std::optional<std::string> description;
  };

  // Helper aliases to represent OpenAPI's common "Object | Reference Object"
  // pattern. Notice that reference must be placed first so it is evaluated
  // initially by the variant parser. Otherwise, we might eagerly parse an
  // object as a reference.
  //
  template <typename T>
  using ref_or = std::variant<reference, T>;

  template <typename T>
  using ref_or_ptr = std::variant<reference, std::shared_ptr<T>>;

  struct server_variable
  {
    std::optional<std::vector<std::string>> enum_;
    std::string default_;
    std::optional<std::string> description;
  };

  struct server
  {
    std::string url;
    std::optional<std::string> description;
    std::optional<std::string> name;
    std::optional<std::map<std::string, server_variable>> variables;
  };

  struct external_documentation
  {
    std::optional<std::string> description;
    std::string url;
  };

  struct tag
  {
    std::string name;
    std::optional<std::string> summary;
    std::optional<std::string> description;
    std::optional<external_documentation> externalDocs;
    std::optional<std::string> parent;
    std::optional<std::string> kind;
  };

  struct xml
  {
    std::optional<std::string> nodeType;
    std::optional<std::string> name;
    std::optional<std::string> namespace_;
    std::optional<std::string> prefix;
    std::optional<bool> attribute;
    std::optional<bool> wrapped;
  };

  struct discriminator
  {
    std::string propertyName;
    std::optional<std::map<std::string, std::string>> mapping;
    std::optional<std::string> defaultMapping;
  };

  struct contact
  {
    std::optional<std::string> name;
    std::optional<std::string> url;
    std::optional<std::string> email;
  };

  struct license
  {
    std::string name;
    std::optional<std::string> identifier;
    std::optional<std::string> url;
  };

  struct info
  {
    std::string title;
    std::optional<std::string> summary;
    std::optional<std::string> description;
    std::optional<std::string> termsOfService;
    std::optional<struct contact> contact;
    std::optional<struct license> license;
    std::string version;
  };

  struct security_requirement
  {
    std::map<std::string, std::vector<std::string>> requirements;
  };

  struct oauth_flow
  {
    std::optional<std::string> authorizationUrl;
    std::optional<std::string> deviceAuthorizationUrl;
    std::optional<std::string> tokenUrl;
    std::optional<std::string> refreshUrl;
    std::map<std::string, std::string> scopes;
  };

  struct oauth_flows
  {
    std::optional<oauth_flow> implicit;
    std::optional<oauth_flow> password;
    std::optional<oauth_flow> clientCredentials;
    std::optional<oauth_flow> authorizationCode;
    std::optional<oauth_flow> deviceAuthorization;
  };

  struct security_scheme
  {
    std::string type;
    std::optional<std::string> description;
    std::optional<std::string> name;
    std::optional<std::string> in;
    std::optional<std::string> scheme;
    std::optional<std::string> bearerFormat;
    std::optional<oauth_flows> flows;
    std::optional<std::string> openIdConnectUrl;
    std::optional<std::string> oauth2MetadataUrl;
    std::optional<bool> deprecated;
  };

  struct example
  {
    std::optional<std::string> summary;
    std::optional<std::string> description;
    std::optional<boost::json::value> dataValue;
    std::optional<std::string> serializedValue;
    std::optional<std::string> externalValue;
    std::optional<boost::json::value> value; // deprecated
  };

  struct schema
  {
    std::optional<std::string> $id;
    std::optional<std::string> $schema;
    std::optional<std::string> $anchor;
    std::optional<std::string> $ref;
    std::optional<std::string> $dynamicAnchor;
    std::optional<std::string> $dynamicRef;
    std::optional<std::map<std::string, std::shared_ptr<schema>>> $defs;
    std::optional<std::string> $comment;
    std::optional<std::vector<std::shared_ptr<schema>>> allOf;
    std::optional<std::vector<std::shared_ptr<schema>>> anyOf;
    std::optional<std::vector<std::shared_ptr<schema>>> oneOf;
    std::shared_ptr<schema> not_;
    std::shared_ptr<schema> if_;
    std::shared_ptr<schema> then_;
    std::shared_ptr<schema> else_;
    std::optional<std::map<std::string, std::shared_ptr<schema>>> dependentSchemas;
    std::optional<std::vector<std::shared_ptr<schema>>> prefixItems;
    std::shared_ptr<schema> items;
    std::shared_ptr<schema> contains;
    std::optional<std::map<std::string, std::shared_ptr<schema>>> properties;
    std::optional<std::map<std::string, std::shared_ptr<schema>>> patternProperties;
    std::optional<std::variant<bool, std::shared_ptr<schema>>> additionalProperties;
    std::shared_ptr<schema> propertyNames;
    std::optional<std::variant<bool, std::shared_ptr<schema>>> unevaluatedItems;
    std::optional<std::variant<bool, std::shared_ptr<schema>>> unevaluatedProperties;
    std::optional<std::variant<std::string, std::vector<std::string>>> type;
    std::optional<std::vector<boost::json::value>> enum_;
    std::optional<boost::json::value> const_;
    std::optional<double> multipleOf;
    std::optional<double> maximum;
    std::optional<double> exclusiveMaximum;
    std::optional<double> minimum;
    std::optional<double> exclusiveMinimum;
    std::optional<size_t> maxLength;
    std::optional<size_t> minLength;
    std::optional<std::string> pattern;
    std::optional<size_t> maxItems;
    std::optional<size_t> minItems;
    std::optional<bool> uniqueItems;
    std::optional<size_t> maxContains;
    std::optional<size_t> minContains;
    std::optional<size_t> maxProperties;
    std::optional<size_t> minProperties;
    std::optional<std::vector<std::string>> required;
    std::optional<std::map<std::string, std::vector<std::string>>> dependentRequired;
    std::optional<std::string> format;
    std::optional<std::string> contentEncoding;
    std::optional<std::string> contentMediaType;
    std::shared_ptr<schema> contentSchema;
    std::optional<std::string> title;
    std::optional<std::string> description;
    std::optional<boost::json::value> default_;
    std::optional<bool> deprecated;
    std::optional<bool> readOnly;
    std::optional<bool> writeOnly;
    std::optional<std::vector<boost::json::value>> examples;
    std::optional<forge::discriminator> discriminator;
    std::optional<forge::xml> xml;
    std::optional<external_documentation> externalDocs;
    std::optional<boost::json::value> example; // deprecated
  };

  struct link
  {
    std::optional<std::string> operationRef;
    std::optional<std::string> operationId;
    std::optional<std::map<std::string, boost::json::value>> parameters;
    std::optional<boost::json::value> requestBody;
    std::optional<std::string> description;
    std::optional<struct server> server;
  };

  struct header
  {
    std::optional<std::string> description;
    std::optional<bool> required;
    std::optional<bool> deprecated;
    std::optional<boost::json::value> example;
    std::optional<std::map<std::string, ref_or<struct example>>> examples;
    std::optional<std::string> style;
    std::optional<bool> explode;
    std::optional<std::shared_ptr<struct schema>> schema;
    std::optional<std::map<std::string, ref_or_ptr<media_type>>> content;
  };

  struct encoding
  {
    std::optional<std::string> contentType;
    std::optional<std::map<std::string, ref_or<header>>> headers;
    std::optional<std::map<std::string, std::shared_ptr<encoding>>> encoding_;
    std::optional<std::vector<std::shared_ptr<encoding>>> prefixEncoding;
    std::shared_ptr<encoding> itemEncoding;
    std::optional<std::string> style;
    std::optional<bool> explode;
    std::optional<bool> allowReserved;
  };

  struct media_type
  {
    std::optional<std::shared_ptr<struct schema>> schema;
    std::optional<std::shared_ptr<struct schema>> itemSchema;
    std::optional<boost::json::value> example;
    std::optional<std::map<std::string, ref_or<struct example>>> examples;
    std::optional<std::map<std::string, struct encoding>> encoding;
    std::optional<std::vector<struct encoding>> prefixEncoding;
    std::optional<std::shared_ptr<struct encoding>> itemEncoding;
  };

  struct parameter
  {
    std::string name;
    std::string in;
    std::optional<std::string> description;
    std::optional<bool> required;
    std::optional<bool> deprecated;
    std::optional<bool> allowEmptyValue;
    std::optional<std::string> style;
    std::optional<bool> explode;
    std::optional<bool> allowReserved;
    std::optional<std::shared_ptr<struct schema>> schema;
    std::optional<boost::json::value> example;
    std::optional<std::map<std::string, ref_or<struct example>>> examples;
    std::optional<std::map<std::string, ref_or<media_type>>> content;
  };

  struct request_body
  {
    std::optional<std::string> description;
    std::map<std::string, ref_or<media_type>> content;
    std::optional<bool> required;
  };

  struct response
  {
    std::string description;
    std::optional<std::map<std::string, ref_or<header>>> headers;
    std::optional<std::map<std::string, ref_or<media_type>>> content;
    std::optional<std::map<std::string, ref_or<link>>> links;
  };

  struct responses
  {
    std::optional<ref_or<response>> default_;
    std::map<std::string, ref_or<response>> responses_;
  };

  struct callback
  {
    std::map<std::string, ref_or_ptr<path_item>> paths;
  };

  struct operation
  {
    std::optional<std::vector<std::string>> tags;
    std::optional<std::string> summary;
    std::optional<std::string> description;
    std::optional<external_documentation> externalDocs;
    std::optional<std::string> operationId;
    std::optional<std::vector<ref_or<parameter>>> parameters;
    std::optional<ref_or<request_body>> requestBody;
    std::optional<forge::responses> responses;
    std::optional<std::map<std::string, ref_or<callback>>> callbacks;
    std::optional<bool> deprecated;
    std::optional<std::vector<security_requirement>> security;
    std::optional<std::vector<server>> servers;
  };

  struct path_item
  {
    std::optional<std::string> $ref;
    std::optional<std::string> summary;
    std::optional<std::string> description;
    std::optional<operation> get;
    std::optional<operation> put;
    std::optional<operation> post;
    std::optional<operation> delete_;
    std::optional<operation> options;
    std::optional<operation> head;
    std::optional<operation> patch;
    std::optional<operation> trace;
    std::optional<operation> query;
    std::optional<std::map<std::string, operation>> additionalOperations;
    std::optional<std::vector<server>> servers;
    std::optional<std::vector<ref_or<parameter>>> parameters;
  };

  struct paths
  {
    std::map<std::string, path_item> items;
  };

  struct components
  {
    std::optional<std::map<std::string, std::shared_ptr<schema>>> schemas;
    std::optional<std::map<std::string, ref_or<response>>> responses;
    std::optional<std::map<std::string, ref_or<parameter>>> parameters;
    std::optional<std::map<std::string, ref_or<example>>> examples;
    std::optional<std::map<std::string, ref_or<request_body>>> requestBodies;
    std::optional<std::map<std::string, ref_or<header>>> headers;
    std::optional<std::map<std::string, ref_or<security_scheme>>>
      securitySchemes;
    std::optional<std::map<std::string, ref_or<link>>> links;
    std::optional<std::map<std::string, ref_or<callback>>> callbacks;
    std::optional<std::map<std::string, path_item>> pathItems;
    std::optional<std::map<std::string, ref_or<media_type>>> mediaTypes;
  };

  struct document
  {
    std::string openapi;
    std::optional<std::string> $self;
    forge::info info;
    std::optional<std::string> jsonSchemaDialect;
    std::optional<std::vector<server>> servers;
    std::optional<forge::paths> paths;
    std::optional<std::map<std::string, path_item>> webhooks;
    std::optional<forge::components> components;
    std::optional<std::vector<security_requirement>> security;
    std::optional<std::vector<tag>> tags;
    std::optional<external_documentation> externalDocs;
  };

  namespace detail
  {
    template <typename T>
    void
    ext (const boost::json::object& o,
         boost::json::string_view k,
         std::optional<T>& v,
         boost::system::error_code& e)
    {
      if (e)
        return;

      if (auto i (o.find (k)); i != o.end () && !i->value ().is_null ())
      {
        auto r (boost::json::try_value_to<T> (i->value ()));

        if (r)
          v = r.value ();
        else
          e = r.error ();
      }
    }

    template <typename T>
    void
    ext (const boost::json::object& o,
         boost::json::string_view k,
         std::shared_ptr<T>& v,
         boost::system::error_code& e)
    {
      if (e)
        return;

      if (auto i (o.find (k)); i != o.end () && !i->value ().is_null ())
      {
        auto r (boost::json::try_value_to<std::shared_ptr<T>> (i->value ()));

        if (r)
          v = r.value ();
        else
          e = r.error ();
      }
    }

    template <typename T>
    void
    ext (const boost::json::object& o,
         boost::json::string_view k,
         T& v,
         boost::system::error_code& e)
    {
      if (e)
        return;

      if (auto i (o.find (k)); i != o.end ())
      {
        auto r (boost::json::try_value_to<T> (i->value ()));

        if (r)
          v = r.value ();
        else
          e = r.error ();
      }
      else
      {
        e = boost::system::errc::make_error_code (
          boost::system::errc::invalid_argument);
      }
    }

    template <typename T>
    void
    set (boost::json::object& o,
         boost::json::string_view k,
         const std::optional<T>& v)
    {
      if (v)
        o[k] = boost::json::value_from (*v);
    }

    template <typename T>
    void
    set (boost::json::object& o,
         boost::json::string_view k,
         const std::shared_ptr<T>& v)
    {
      if (v)
        o[k] = boost::json::value_from (*v);
    }

    template <typename T>
    void
    set (boost::json::object& o, boost::json::string_view k, const T& v)
    {
      o[k] = boost::json::value_from (v);
    }
  }

  template <typename T>
  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const std::shared_ptr<T>& v)
  {
    if (v)
      j = boost::json::value_from (*v);
    else
      j = boost::json::value (nullptr);
  }

  template <typename T>
  inline boost::system::result<std::shared_ptr<T>>
  tag_invoke (boost::json::try_value_to_tag<std::shared_ptr<T>>,
              const boost::json::value& j)
  {
    if (j.is_null ())
      return std::shared_ptr<T> (nullptr);

    auto r (boost::json::try_value_to<T> (j));

    if (!r)
      return r.error ();

    return std::make_shared<T> (r.value ());
  }

  inline boost::system::result<reference>
  tag_invoke (boost::json::try_value_to_tag<reference>,
              const boost::json::value& j)
  {
    if (!j.is_object () || !j.as_object ().contains ("$ref"))
      return boost::system::errc::make_error_code (
        boost::system::errc::invalid_argument);

    reference v;
    boost::system::error_code e;
    const auto& o (j.as_object ());

    detail::ext (o, "$ref", v.$ref, e);
    detail::ext (o, "summary", v.summary, e);
    detail::ext (o, "description", v.description, e);

    if (e)
      return e;

    return v;
  }

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const reference& v)
  {
    boost::json::object o;

    detail::set (o, "$ref", v.$ref);
    detail::set (o, "summary", v.summary);
    detail::set (o, "description", v.description);

    j = std::move (o);
  }

#define FORGE_JSON_EXT_SET(T, To, From)                                        \
  inline void tag_invoke (boost::json::value_from_tag,                         \
                          boost::json::value& j,                               \
                          const T& v)                                          \
  {                                                                            \
    boost::json::object o;                                                     \
    To j = std::move (o);                                                      \
  }                                                                            \
                                                                               \
  inline boost::system::result<T> tag_invoke (                                 \
    boost::json::try_value_to_tag<T>,                                          \
    const boost::json::value& j)                                               \
  {                                                                            \
    if (!j.is_object ())                                                       \
      return boost::system::errc::make_error_code (                            \
        boost::system::errc::invalid_argument);                                \
    T v;                                                                       \
    boost::system::error_code e;                                               \
    const auto& o (j.as_object ());                                            \
    From if (e) return e;                                                      \
    return v;                                                                  \
  }

  FORGE_JSON_EXT_SET (
    server_variable,
    {
      detail::set (o, "enum", v.enum_);
      detail::set (o, "default", v.default_);
      detail::set (o, "description", v.description);
    },
    {
      detail::ext (o, "enum", v.enum_, e);
      detail::ext (o, "default", v.default_, e);
      detail::ext (o, "description", v.description, e);
    })

  FORGE_JSON_EXT_SET (
    server,
    {
      detail::set (o, "url", v.url);
      detail::set (o, "description", v.description);
      detail::set (o, "name", v.name);
      detail::set (o, "variables", v.variables);
    },
    {
      detail::ext (o, "url", v.url, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "variables", v.variables, e);
    })

  FORGE_JSON_EXT_SET (
    external_documentation,
    {
      detail::set (o, "description", v.description);
      detail::set (o, "url", v.url);
    },
    {
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "url", v.url, e);
    })

  FORGE_JSON_EXT_SET (
    tag,
    {
      detail::set (o, "name", v.name);
      detail::set (o, "summary", v.summary);
      detail::set (o, "description", v.description);
      detail::set (o, "externalDocs", v.externalDocs);
      detail::set (o, "parent", v.parent);
      detail::set (o, "kind", v.kind);
    },
    {
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "summary", v.summary, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "externalDocs", v.externalDocs, e);
      detail::ext (o, "parent", v.parent, e);
      detail::ext (o, "kind", v.kind, e);
    })

  FORGE_JSON_EXT_SET (
    xml,
    {
      detail::set (o, "nodeType", v.nodeType);
      detail::set (o, "name", v.name);
      detail::set (o, "namespace", v.namespace_);
      detail::set (o, "prefix", v.prefix);
      detail::set (o, "attribute", v.attribute);
      detail::set (o, "wrapped", v.wrapped);
    },
    {
      detail::ext (o, "nodeType", v.nodeType, e);
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "namespace", v.namespace_, e);
      detail::ext (o, "prefix", v.prefix, e);
      detail::ext (o, "attribute", v.attribute, e);
      detail::ext (o, "wrapped", v.wrapped, e);
    })

  FORGE_JSON_EXT_SET (
    discriminator,
    {
      detail::set (o, "propertyName", v.propertyName);
      detail::set (o, "mapping", v.mapping);
      detail::set (o, "defaultMapping", v.defaultMapping);
    },
    {
      detail::ext (o, "propertyName", v.propertyName, e);
      detail::ext (o, "mapping", v.mapping, e);
      detail::ext (o, "defaultMapping", v.defaultMapping, e);
    })

  FORGE_JSON_EXT_SET (
    contact,
    {
      detail::set (o, "name", v.name);
      detail::set (o, "url", v.url);
      detail::set (o, "email", v.email);
    },
    {
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "url", v.url, e);
      detail::ext (o, "email", v.email, e);
    })

  FORGE_JSON_EXT_SET (
    license,
    {
      detail::set (o, "name", v.name);
      detail::set (o, "identifier", v.identifier);
      detail::set (o, "url", v.url);
    },
    {
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "identifier", v.identifier, e);
      detail::ext (o, "url", v.url, e);
    })

  FORGE_JSON_EXT_SET (
    info,
    {
      detail::set (o, "title", v.title);
      detail::set (o, "summary", v.summary);
      detail::set (o, "description", v.description);
      detail::set (o, "termsOfService", v.termsOfService);
      detail::set (o, "contact", v.contact);
      detail::set (o, "license", v.license);
      detail::set (o, "version", v.version);
    },
    {
      detail::ext (o, "title", v.title, e);
      detail::ext (o, "summary", v.summary, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "termsOfService", v.termsOfService, e);
      detail::ext (o, "contact", v.contact, e);
      detail::ext (o, "license", v.license, e);
      detail::ext (o, "version", v.version, e);
    })

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const security_requirement& v)
  {
    j = boost::json::value_from (v.requirements);
  }

  inline boost::system::result<security_requirement>
  tag_invoke (boost::json::try_value_to_tag<security_requirement>,
              const boost::json::value& j)
  {
    auto r (
      boost::json::try_value_to<std::map<std::string,
                                         std::vector<std::string>>> (j));

    if (!r)
      return r.error ();

    return security_requirement (r.value ());
  }

  FORGE_JSON_EXT_SET (
    oauth_flow,
    {
      detail::set (o, "authorizationUrl", v.authorizationUrl);
      detail::set (o, "deviceAuthorizationUrl", v.deviceAuthorizationUrl);
      detail::set (o, "tokenUrl", v.tokenUrl);
      detail::set (o, "refreshUrl", v.refreshUrl);
      detail::set (o, "scopes", v.scopes);
    },
    {
      detail::ext (o, "authorizationUrl", v.authorizationUrl, e);
      detail::ext (o, "deviceAuthorizationUrl", v.deviceAuthorizationUrl, e);
      detail::ext (o, "tokenUrl", v.tokenUrl, e);
      detail::ext (o, "refreshUrl", v.refreshUrl, e);
      detail::ext (o, "scopes", v.scopes, e);
    })

  FORGE_JSON_EXT_SET (
    oauth_flows,
    {
      detail::set (o, "implicit", v.implicit);
      detail::set (o, "password", v.password);
      detail::set (o, "clientCredentials", v.clientCredentials);
      detail::set (o, "authorizationCode", v.authorizationCode);
      detail::set (o, "deviceAuthorization", v.deviceAuthorization);
    },
    {
      detail::ext (o, "implicit", v.implicit, e);
      detail::ext (o, "password", v.password, e);
      detail::ext (o, "clientCredentials", v.clientCredentials, e);
      detail::ext (o, "authorizationCode", v.authorizationCode, e);
      detail::ext (o, "deviceAuthorization", v.deviceAuthorization, e);
    })

  FORGE_JSON_EXT_SET (
    security_scheme,
    {
      detail::set (o, "type", v.type);
      detail::set (o, "description", v.description);
      detail::set (o, "name", v.name);
      detail::set (o, "in", v.in);
      detail::set (o, "scheme", v.scheme);
      detail::set (o, "bearerFormat", v.bearerFormat);
      detail::set (o, "flows", v.flows);
      detail::set (o, "openIdConnectUrl", v.openIdConnectUrl);
      detail::set (o, "oauth2MetadataUrl", v.oauth2MetadataUrl);
      detail::set (o, "deprecated", v.deprecated);
    },
    {
      detail::ext (o, "type", v.type, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "in", v.in, e);
      detail::ext (o, "scheme", v.scheme, e);
      detail::ext (o, "bearerFormat", v.bearerFormat, e);
      detail::ext (o, "flows", v.flows, e);
      detail::ext (o, "openIdConnectUrl", v.openIdConnectUrl, e);
      detail::ext (o, "oauth2MetadataUrl", v.oauth2MetadataUrl, e);
      detail::ext (o, "deprecated", v.deprecated, e);
    })

  FORGE_JSON_EXT_SET (
    example,
    {
      detail::set (o, "summary", v.summary);
      detail::set (o, "description", v.description);
      detail::set (o, "dataValue", v.dataValue);
      detail::set (o, "serializedValue", v.serializedValue);
      detail::set (o, "externalValue", v.externalValue);
      detail::set (o, "value", v.value);
    },
    {
      detail::ext (o, "summary", v.summary, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "dataValue", v.dataValue, e);
      detail::ext (o, "serializedValue", v.serializedValue, e);
      detail::ext (o, "externalValue", v.externalValue, e);
      detail::ext (o, "value", v.value, e);
    })

  FORGE_JSON_EXT_SET (
    schema,
    {
      detail::set (o, "$id", v.$id);
      detail::set (o, "$schema", v.$schema);
      detail::set (o, "$anchor", v.$anchor);
      detail::set (o, "$ref", v.$ref);
      detail::set (o, "$dynamicAnchor", v.$dynamicAnchor);
      detail::set (o, "$dynamicRef", v.$dynamicRef);
      detail::set (o, "$defs", v.$defs);
      detail::set (o, "$comment", v.$comment);
      detail::set (o, "allOf", v.allOf);
      detail::set (o, "anyOf", v.anyOf);
      detail::set (o, "oneOf", v.oneOf);
      detail::set (o, "not", v.not_);
      detail::set (o, "if", v.if_);
      detail::set (o, "then", v.then_);
      detail::set (o, "else", v.else_);
      detail::set (o, "dependentSchemas", v.dependentSchemas);
      detail::set (o, "prefixItems", v.prefixItems);
      detail::set (o, "items", v.items);
      detail::set (o, "contains", v.contains);
      detail::set (o, "properties", v.properties);
      detail::set (o, "patternProperties", v.patternProperties);
      detail::set (o, "additionalProperties", v.additionalProperties);
      detail::set (o, "propertyNames", v.propertyNames);
      detail::set (o, "unevaluatedItems", v.unevaluatedItems);
      detail::set (o, "unevaluatedProperties", v.unevaluatedProperties);
      detail::set (o, "type", v.type);
      detail::set (o, "enum", v.enum_);
      detail::set (o, "const", v.const_);
      detail::set (o, "multipleOf", v.multipleOf);
      detail::set (o, "maximum", v.maximum);
      detail::set (o, "exclusiveMaximum", v.exclusiveMaximum);
      detail::set (o, "minimum", v.minimum);
      detail::set (o, "exclusiveMinimum", v.exclusiveMinimum);
      detail::set (o, "maxLength", v.maxLength);
      detail::set (o, "minLength", v.minLength);
      detail::set (o, "pattern", v.pattern);
      detail::set (o, "maxItems", v.maxItems);
      detail::set (o, "minItems", v.minItems);
      detail::set (o, "uniqueItems", v.uniqueItems);
      detail::set (o, "maxContains", v.maxContains);
      detail::set (o, "minContains", v.minContains);
      detail::set (o, "maxProperties", v.maxProperties);
      detail::set (o, "minProperties", v.minProperties);
      detail::set (o, "required", v.required);
      detail::set (o, "dependentRequired", v.dependentRequired);
      detail::set (o, "format", v.format);
      detail::set (o, "contentEncoding", v.contentEncoding);
      detail::set (o, "contentMediaType", v.contentMediaType);
      detail::set (o, "contentSchema", v.contentSchema);
      detail::set (o, "title", v.title);
      detail::set (o, "description", v.description);
      detail::set (o, "default", v.default_);
      detail::set (o, "deprecated", v.deprecated);
      detail::set (o, "readOnly", v.readOnly);
      detail::set (o, "writeOnly", v.writeOnly);
      detail::set (o, "examples", v.examples);
      detail::set (o, "discriminator", v.discriminator);
      detail::set (o, "xml", v.xml);
      detail::set (o, "externalDocs", v.externalDocs);
      detail::set (o, "example", v.example);
    },
    {
      detail::ext (o, "$id", v.$id, e);
      detail::ext (o, "$schema", v.$schema, e);
      detail::ext (o, "$anchor", v.$anchor, e);
      detail::ext (o, "$ref", v.$ref, e);
      detail::ext (o, "$dynamicAnchor", v.$dynamicAnchor, e);
      detail::ext (o, "$dynamicRef", v.$dynamicRef, e);
      detail::ext (o, "$defs", v.$defs, e);
      detail::ext (o, "$comment", v.$comment, e);
      detail::ext (o, "allOf", v.allOf, e);
      detail::ext (o, "anyOf", v.anyOf, e);
      detail::ext (o, "oneOf", v.oneOf, e);
      detail::ext (o, "not", v.not_, e);
      detail::ext (o, "if", v.if_, e);
      detail::ext (o, "then", v.then_, e);
      detail::ext (o, "else", v.else_, e);
      detail::ext (o, "dependentSchemas", v.dependentSchemas, e);
      detail::ext (o, "prefixItems", v.prefixItems, e);
      detail::ext (o, "items", v.items, e);
      detail::ext (o, "contains", v.contains, e);
      detail::ext (o, "properties", v.properties, e);
      detail::ext (o, "patternProperties", v.patternProperties, e);
      detail::ext (o, "additionalProperties", v.additionalProperties, e);
      detail::ext (o, "propertyNames", v.propertyNames, e);
      detail::ext (o, "unevaluatedItems", v.unevaluatedItems, e);
      detail::ext (o, "unevaluatedProperties", v.unevaluatedProperties, e);
      detail::ext (o, "type", v.type, e);
      detail::ext (o, "enum", v.enum_, e);
      detail::ext (o, "const", v.const_, e);
      detail::ext (o, "multipleOf", v.multipleOf, e);
      detail::ext (o, "maximum", v.maximum, e);
      detail::ext (o, "exclusiveMaximum", v.exclusiveMaximum, e);
      detail::ext (o, "minimum", v.minimum, e);
      detail::ext (o, "exclusiveMinimum", v.exclusiveMinimum, e);
      detail::ext (o, "maxLength", v.maxLength, e);
      detail::ext (o, "minLength", v.minLength, e);
      detail::ext (o, "pattern", v.pattern, e);
      detail::ext (o, "maxItems", v.maxItems, e);
      detail::ext (o, "minItems", v.minItems, e);
      detail::ext (o, "uniqueItems", v.uniqueItems, e);
      detail::ext (o, "maxContains", v.maxContains, e);
      detail::ext (o, "minContains", v.minContains, e);
      detail::ext (o, "maxProperties", v.maxProperties, e);
      detail::ext (o, "minProperties", v.minProperties, e);
      detail::ext (o, "required", v.required, e);
      detail::ext (o, "dependentRequired", v.dependentRequired, e);
      detail::ext (o, "format", v.format, e);
      detail::ext (o, "contentEncoding", v.contentEncoding, e);
      detail::ext (o, "contentMediaType", v.contentMediaType, e);
      detail::ext (o, "contentSchema", v.contentSchema, e);
      detail::ext (o, "title", v.title, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "default", v.default_, e);
      detail::ext (o, "deprecated", v.deprecated, e);
      detail::ext (o, "readOnly", v.readOnly, e);
      detail::ext (o, "writeOnly", v.writeOnly, e);
      detail::ext (o, "examples", v.examples, e);
      detail::ext (o, "discriminator", v.discriminator, e);
      detail::ext (o, "xml", v.xml, e);
      detail::ext (o, "externalDocs", v.externalDocs, e);
      detail::ext (o, "example", v.example, e);
    })

  FORGE_JSON_EXT_SET (
    link,
    {
      detail::set (o, "operationRef", v.operationRef);
      detail::set (o, "operationId", v.operationId);
      detail::set (o, "parameters", v.parameters);
      detail::set (o, "requestBody", v.requestBody);
      detail::set (o, "description", v.description);
      detail::set (o, "server", v.server);
    },
    {
      detail::ext (o, "operationRef", v.operationRef, e);
      detail::ext (o, "operationId", v.operationId, e);
      detail::ext (o, "parameters", v.parameters, e);
      detail::ext (o, "requestBody", v.requestBody, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "server", v.server, e);
    })

  FORGE_JSON_EXT_SET (
    header,
    {
      detail::set (o, "description", v.description);
      detail::set (o, "required", v.required);
      detail::set (o, "deprecated", v.deprecated);
      detail::set (o, "example", v.example);
      detail::set (o, "examples", v.examples);
      detail::set (o, "style", v.style);
      detail::set (o, "explode", v.explode);
      detail::set (o, "schema", v.schema);
      detail::set (o, "content", v.content);
    },
    {
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "required", v.required, e);
      detail::ext (o, "deprecated", v.deprecated, e);
      detail::ext (o, "example", v.example, e);
      detail::ext (o, "examples", v.examples, e);
      detail::ext (o, "style", v.style, e);
      detail::ext (o, "explode", v.explode, e);
      detail::ext (o, "schema", v.schema, e);
      detail::ext (o, "content", v.content, e);
    })

  FORGE_JSON_EXT_SET (
    encoding,
    {
      detail::set (o, "contentType", v.contentType);
      detail::set (o, "headers", v.headers);
      detail::set (o, "encoding", v.encoding_);
      detail::set (o, "prefixEncoding", v.prefixEncoding);
      detail::set (o, "itemEncoding", v.itemEncoding);
      detail::set (o, "style", v.style);
      detail::set (o, "explode", v.explode);
      detail::set (o, "allowReserved", v.allowReserved);
    },
    {
      detail::ext (o, "contentType", v.contentType, e);
      detail::ext (o, "headers", v.headers, e);
      detail::ext (o, "encoding", v.encoding_, e);
      detail::ext (o, "prefixEncoding", v.prefixEncoding, e);
      detail::ext (o, "itemEncoding", v.itemEncoding, e);
      detail::ext (o, "style", v.style, e);
      detail::ext (o, "explode", v.explode, e);
      detail::ext (o, "allowReserved", v.allowReserved, e);
    })

  FORGE_JSON_EXT_SET (
    media_type,
    {
      detail::set (o, "schema", v.schema);
      detail::set (o, "itemSchema", v.itemSchema);
      detail::set (o, "example", v.example);
      detail::set (o, "examples", v.examples);
      detail::set (o, "encoding", v.encoding);
      detail::set (o, "prefixEncoding", v.prefixEncoding);
      detail::set (o, "itemEncoding", v.itemEncoding);
    },
    {
      detail::ext (o, "schema", v.schema, e);
      detail::ext (o, "itemSchema", v.itemSchema, e);
      detail::ext (o, "example", v.example, e);
      detail::ext (o, "examples", v.examples, e);
      detail::ext (o, "encoding", v.encoding, e);
      detail::ext (o, "prefixEncoding", v.prefixEncoding, e);
      detail::ext (o, "itemEncoding", v.itemEncoding, e);
    })

  FORGE_JSON_EXT_SET (
    parameter,
    {
      detail::set (o, "name", v.name);
      detail::set (o, "in", v.in);
      detail::set (o, "description", v.description);
      detail::set (o, "required", v.required);
      detail::set (o, "deprecated", v.deprecated);
      detail::set (o, "allowEmptyValue", v.allowEmptyValue);
      detail::set (o, "style", v.style);
      detail::set (o, "explode", v.explode);
      detail::set (o, "allowReserved", v.allowReserved);
      detail::set (o, "schema", v.schema);
      detail::set (o, "example", v.example);
      detail::set (o, "examples", v.examples);
      detail::set (o, "content", v.content);
    },
    {
      detail::ext (o, "name", v.name, e);
      detail::ext (o, "in", v.in, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "required", v.required, e);
      detail::ext (o, "deprecated", v.deprecated, e);
      detail::ext (o, "allowEmptyValue", v.allowEmptyValue, e);
      detail::ext (o, "style", v.style, e);
      detail::ext (o, "explode", v.explode, e);
      detail::ext (o, "allowReserved", v.allowReserved, e);
      detail::ext (o, "schema", v.schema, e);
      detail::ext (o, "example", v.example, e);
      detail::ext (o, "examples", v.examples, e);
      detail::ext (o, "content", v.content, e);
    })

  FORGE_JSON_EXT_SET (
    request_body,
    {
      detail::set (o, "description", v.description);
      detail::set (o, "content", v.content);
      detail::set (o, "required", v.required);
    },
    {
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "content", v.content, e);
      detail::ext (o, "required", v.required, e);
    })

  FORGE_JSON_EXT_SET (
    response,
    {
      detail::set (o, "description", v.description);
      detail::set (o, "headers", v.headers);
      detail::set (o, "content", v.content);
      detail::set (o, "links", v.links);
    },
    {
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "headers", v.headers, e);
      detail::ext (o, "content", v.content, e);
      detail::ext (o, "links", v.links, e);
    })

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const responses& v)
  {
    boost::json::object o;

    if (v.default_)
    {
      o["default"] = boost::json::value_from (*v.default_);
    }

    for (const auto& [k, vl] : v.responses_)
    {
      o[k] = boost::json::value_from (vl);
    }

    j = std::move (o);
  }

  inline boost::system::result<responses>
  tag_invoke (boost::json::try_value_to_tag<responses>,
              const boost::json::value& j)
  {
    if (!j.is_object ())
      return boost::system::errc::make_error_code (
        boost::system::errc::invalid_argument);

    responses v;
    const auto& o (j.as_object ());

    for (const auto& [k, vl] : o)
    {
      if (k == "default")
      {
        auto r (boost::json::try_value_to<ref_or<response>> (vl));

        if (r)
          v.default_ = r.value ();
        else
          return r.error ();
      }
      else if (!k.starts_with ("x-"))
      {
        auto r (boost::json::try_value_to<ref_or<response>> (vl));

        if (r)
          v.responses_[std::string (k)] = r.value ();
        else
          return r.error ();
      }
    }

    return v;
  }

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const callback& v)
  {
    j = boost::json::value_from (v.paths);
  }

  inline boost::system::result<callback>
  tag_invoke (boost::json::try_value_to_tag<callback>,
              const boost::json::value& j)
  {
    auto r (boost::json::try_value_to<std::map<std::string,
                                               ref_or_ptr<path_item>>> (j));

    if (!r)
      return r.error ();

    return callback (r.value ());
  }

  FORGE_JSON_EXT_SET (
    operation,
    {
      detail::set (o, "tags", v.tags);
      detail::set (o, "summary", v.summary);
      detail::set (o, "description", v.description);
      detail::set (o, "externalDocs", v.externalDocs);
      detail::set (o, "operationId", v.operationId);
      detail::set (o, "parameters", v.parameters);
      detail::set (o, "requestBody", v.requestBody);
      detail::set (o, "responses", v.responses);
      detail::set (o, "callbacks", v.callbacks);
      detail::set (o, "deprecated", v.deprecated);
      detail::set (o, "security", v.security);
      detail::set (o, "servers", v.servers);
    },
    {
      detail::ext (o, "tags", v.tags, e);
      detail::ext (o, "summary", v.summary, e);
      detail::ext (o, "description", v.description, e);
      detail::ext (o, "externalDocs", v.externalDocs, e);
      detail::ext (o, "operationId", v.operationId, e);
      detail::ext (o, "parameters", v.parameters, e);
      detail::ext (o, "requestBody", v.requestBody, e);
      detail::ext (o, "responses", v.responses, e);
      detail::ext (o, "callbacks", v.callbacks, e);
      detail::ext (o, "deprecated", v.deprecated, e);
      detail::ext (o, "security", v.security, e);
      detail::ext (o, "servers", v.servers, e);
    })

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const path_item& v)
  {
    boost::json::object o;

    detail::set (o, "$ref", v.$ref);
    detail::set (o, "summary", v.summary);
    detail::set (o, "description", v.description);
    detail::set (o, "get", v.get);
    detail::set (o, "put", v.put);
    detail::set (o, "post", v.post);
    detail::set (o, "delete", v.delete_);
    detail::set (o, "options", v.options);
    detail::set (o, "head", v.head);
    detail::set (o, "patch", v.patch);
    detail::set (o, "trace", v.trace);
    detail::set (o, "query", v.query);
    detail::set (o, "servers", v.servers);
    detail::set (o, "parameters", v.parameters);

    if (v.additionalOperations)
    {
      for (const auto& [m, op] : *v.additionalOperations)
      {
        o[m] = boost::json::value_from (op);
      }
    }

    j = std::move (o);
  }

  inline boost::system::result<path_item>
  tag_invoke (boost::json::try_value_to_tag<path_item>,
              const boost::json::value& j)
  {
    if (!j.is_object ())
      return boost::system::errc::make_error_code (
        boost::system::errc::invalid_argument);

    path_item v;
    boost::system::error_code e;
    const auto& o (j.as_object ());

    detail::ext (o, "$ref", v.$ref, e);
    detail::ext (o, "summary", v.summary, e);
    detail::ext (o, "description", v.description, e);
    detail::ext (o, "get", v.get, e);
    detail::ext (o, "put", v.put, e);
    detail::ext (o, "post", v.post, e);
    detail::ext (o, "delete", v.delete_, e);
    detail::ext (o, "options", v.options, e);
    detail::ext (o, "head", v.head, e);
    detail::ext (o, "patch", v.patch, e);
    detail::ext (o, "trace", v.trace, e);
    detail::ext (o, "query", v.query, e);
    detail::ext (o, "servers", v.servers, e);
    detail::ext (o, "parameters", v.parameters, e);

    if (e)
      return e;

    std::map<std::string, operation> a;

    for (const auto& [k, vl] : o)
    {
      if (!k.starts_with ("x-") && k != "$ref" && k != "summary" &&
          k != "description" && k != "get" && k != "put" && k != "post" &&
          k != "delete" && k != "options" && k != "head" && k != "patch" &&
          k != "trace" && k != "query" && k != "servers" && k != "parameters")
      {
        auto r (boost::json::try_value_to<operation> (vl));

        if (r)
          a[std::string (k)] = r.value ();
        else
          return r.error ();
      }
    }

    if (!a.empty ())
      v.additionalOperations = std::move (a);

    return v;
  }

  inline void
  tag_invoke (boost::json::value_from_tag,
              boost::json::value& j,
              const paths& v)
  {
    j = boost::json::value_from (v.items);
  }

  inline boost::system::result<paths>
  tag_invoke (boost::json::try_value_to_tag<paths>,
              const boost::json::value& j)
  {
    auto r (boost::json::try_value_to<std::map<std::string, path_item>> (j));

    if (!r)
      return r.error ();

    return paths (r.value ());
  }

  FORGE_JSON_EXT_SET (
    components,
    {
      detail::set (o, "schemas", v.schemas);
      detail::set (o, "responses", v.responses);
      detail::set (o, "parameters", v.parameters);
      detail::set (o, "examples", v.examples);
      detail::set (o, "requestBodies", v.requestBodies);
      detail::set (o, "headers", v.headers);
      detail::set (o, "securitySchemes", v.securitySchemes);
      detail::set (o, "links", v.links);
      detail::set (o, "callbacks", v.callbacks);
      detail::set (o, "pathItems", v.pathItems);
      detail::set (o, "mediaTypes", v.mediaTypes);
    },
    {
      detail::ext (o, "schemas", v.schemas, e);
      detail::ext (o, "responses", v.responses, e);
      detail::ext (o, "parameters", v.parameters, e);
      detail::ext (o, "examples", v.examples, e);
      detail::ext (o, "requestBodies", v.requestBodies, e);
      detail::ext (o, "headers", v.headers, e);
      detail::ext (o, "securitySchemes", v.securitySchemes, e);
      detail::ext (o, "links", v.links, e);
      detail::ext (o, "callbacks", v.callbacks, e);
      detail::ext (o, "pathItems", v.pathItems, e);
      detail::ext (o, "mediaTypes", v.mediaTypes, e);
    })

  FORGE_JSON_EXT_SET (
    document,
    {
      detail::set (o, "openapi", v.openapi);
      detail::set (o, "$self", v.$self);
      detail::set (o, "info", v.info);
      detail::set (o, "jsonSchemaDialect", v.jsonSchemaDialect);
      detail::set (o, "servers", v.servers);
      detail::set (o, "paths", v.paths);
      detail::set (o, "webhooks", v.webhooks);
      detail::set (o, "components", v.components);
      detail::set (o, "security", v.security);
      detail::set (o, "tags", v.tags);
      detail::set (o, "externalDocs", v.externalDocs);
    },
    {
      detail::ext (o, "openapi", v.openapi, e);
      detail::ext (o, "$self", v.$self, e);
      detail::ext (o, "info", v.info, e);
      detail::ext (o, "jsonSchemaDialect", v.jsonSchemaDialect, e);
      detail::ext (o, "servers", v.servers, e);
      detail::ext (o, "paths", v.paths, e);
      detail::ext (o, "webhooks", v.webhooks, e);
      detail::ext (o, "components", v.components, e);
      detail::ext (o, "security", v.security, e);
      detail::ext (o, "tags", v.tags, e);
      detail::ext (o, "externalDocs", v.externalDocs, e);
    })

#undef FORGE_JSON_EXT_SET
}
