#include "modrequestadapter.hpp"

const RequestAdapter::METHODE RequestAdapter::RequestAdapterGetMethode(std::string &methode)
{
  string2  mmethod = (std::string)methode;

  switch (mmethod.is_in_list(m_httpreq.m_method))
  {
  case 1:
  return (OPTIONS);
  break;
  case 2:
  return (GET);
  break;
  case 3:
  return (HEAD);
  break;
  case 4:
  return (POST);
  break;
  case 5:
  return (PUT);
  break;
  case 6:
  return (DEL);
  break;
  case 7:
  return (TRACE);
  break;
  case 8:
  return (CONNECT);
  break;

  default :
  break;
  }
}

RequestAdapter::RequestAdapter(HttpRequest &httpreq)
: m_httpreq(httpreq)
{

  m_httpmethod = RequestAdapterGetMethode(m_httpreq.m_http_map["method"]);
}

bool RequestAdapter::setMethode(const METHODE &method)
{
  m_httpmethod = method;
  m_httpreq.m_http_map["method"] = m_httpreq.m_method[method + 1];

  return (1);
}

bool RequestAdapter::setRequestURI(const std::string &str)
{
  m_httpreq.m_http_map["uri"] = str;
  

  return (1);
}

bool RequestAdapter::setHTTPVersion(const std::string &version)
{
  m_httpreq.m_http_map["version"] = version;
  
  return (1);
}


bool RequestAdapter::setHeader(const std::string &key, const std::string &value)
{
  m_httpreq.m_http_map[key] = value;
  m_headers[key] = value;

  return (1);
}

bool RequestAdapter::setHeaders(const MHEADERS &newmap)
{
  m_headers = newmap;

  return (1);
}

bool RequestAdapter::delHeader(const std::string &key)
{
  m_httpreq.m_http_map[key] = "";
  m_headers[key] = "";


  return (1);
}

const RequestAdapter::METHODE &RequestAdapter::getMethode() const
{
  return (m_httpmethod);
}

const std::string&  RequestAdapter::getRequestURI(void) const
{
  return (m_httpreq.m_http_map["uri"]);
}
const std::string&  RequestAdapter::getHTTPVersion(void) const
{
  return (m_httpreq.m_http_map["version"]);
}

const MHEADERS&     RequestAdapter::getHeaders(void) const
{
  return (m_headers);
}

const std::string&  RequestAdapter::getHeader(const std::string&key) const
{
  return (m_httpreq.m_http_map[key]);
}
