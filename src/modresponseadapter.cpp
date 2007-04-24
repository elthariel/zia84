#include "modresponseadapter.hpp"

ResponseAdapter::ResponseAdapter(HttpRequest &httpreq)
: m_httpreq(httpreq)
{
 m_code = m_httpreq.m_status;
 m_httpreq.m_http_map["reason"] = m_httpreq.m_reason[m_code].reason;
}

bool	ResponseAdapter::setHTTPVersion(const std::string &version)
{
  m_httpreq.m_http_map["version"] = version;
  
  return (1);
}

bool 	ResponseAdapter::setStatusCode(const CODE &code)
{
  m_httpreq.m_status = code;
  m_code = code;
  return (1);
}

bool	ResponseAdapter::setReasonPhrase(const std::string &reason)
{
  m_httpreq.m_http_map["reason"] = reason;
  return (1);
}

bool	ResponseAdapter::setHeader(const std::string &key, const std::string&value)
{ 
  m_httpreq.m_http_map[key] = value;
  m_headers[key] = value;

  return (1);
}

bool	ResponseAdapter::setHeaders(const MHEADERS &newmap)
{
  m_headers = newmap;

  return (1);
}

bool 	ResponseAdapter::delHeader(const std::string&key)
{  
  m_httpreq.m_http_map[key] = "";
  m_headers[key] = "";

  return (1);
}

const std::string&  ResponseAdapter::getHTTPVersion(void) const
{
  return (m_httpreq.m_http_map["version"]);
}

const CODE&         ResponseAdapter::getStatusCode(void) const
{
   return (m_httpreq.m_status);
}

const std::string&  ResponseAdapter::getReasonPhrase(void) const
{
  return (m_httpreq.m_http_map["reason"]); 
}

const MHEADERS&     ResponseAdapter::getHeaders(void) const
{
  return (m_headers);
}

const std::string&  ResponseAdapter::getHeader(const std::string &key) const
{
  return (m_httpreq.m_http_map[key]);
}

