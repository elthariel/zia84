#include "modrawadapter.hpp"



RawAdapter::RawAdapter(HttpRequest &httpreq, int id, EZ_IBasicRawBuffer::RAWTYPE type)
: m_httpreq(httpreq), m_id(id), m_rawtype(type)
{
  if (m_rawtype == EZ_IBasicRawBuffer::RESPONSE)
  {
    m_rawlen = m_httpreq.m_data.length();
    m_rawtype = EZ_IBasicRawBuffer::RESPONSE;
  }
  else
  {
    m_rawlen = m_httpreq.m_body.length();
    m_rawtype = EZ_IBasicRawBuffer::REQUEST;
  }
}

EZ_IBuffer::TYPE    RawAdapter::getBufferType() const
{
  return EZ_IBuffer::EZ_RAW;
}
/*
bool   RawAdapter::setRawType(const EZ_IBasicRawBuffer::RAWTYPE&)
{
  return false;
}

const EZ_IBasicRawBuffer::RAWTYPE&  RawAdapter::getRawType(void) const
{
}
*/
bool	RawAdapter::setData(const std::string& data)
{
  if (m_rawtype == EZ_IBasicRawBuffer::RESPONSE)
    m_httpreq.m_data = data;
  else
    m_httpreq.m_body = data;
  return (1);
}

bool	RawAdapter::setLength(const int &length)
{
  string2 len;

  len.itoa(length);
  m_httpreq.m_http_map["content-length"] = len;
  m_rawlen = length;
  return (1);
}

const std::string&  RawAdapter::getData(void) const
{
  if (m_rawtype == EZ_IBasicRawBuffer::RESPONSE)
    return (m_httpreq.m_body); //set une varibale plutot ?
  else
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
   return (1);
}


const	int& RawAdapter::getRequestID(void) const
{
  return (m_id);
}

void	RawAdapter::setRequestID(const int &newid)
{
  m_id = newid;
}
