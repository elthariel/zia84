#include "modrawadapter.hpp"

RawAdapter::RawAdapter(HttpRequest &httpreq)
: m_httpreq(httpreq)
{
  if (m_httpreq.m_http_map["rawtype"].compare("RESPONSE")) 
    m_rawtype = EZ_IBasicRawBuffer::RESPONSE;
  else
    m_rawtype = EZ_IBasicRawBuffer::REQUEST;
}

bool	RawAdapter::setData(const std::string& data)
{
  m_httpreq.m_data = data;
  return (1);
}

bool	RawAdapter::setLength(const int &length)
{
  string2 len;

  len.itoa(length);
  m_httpreq.m_http_map["datalength"] = len;
  m_rawlen = length;
  return (1);
}

const std::string&  RawAdapter::getData(void) const
{
  return (m_httpreq.m_data);
}

const int& RawAdapter::getLength(void) const
{
  return (m_rawlen);
}

const EZ_IBasicRawBuffer::RAWTYPE&  RawAdapter::getRawType(void) const
{
  return (m_rawtype);
}

bool  RawAdapter::setRawType(const EZ_IBasicRawBuffer::RAWTYPE& newtype)
{
   m_rawtype = newtype;
}
