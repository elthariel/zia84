#ifndef __MODRAWADAPTER_H__
#define __MODRAWADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"


class RawAdapter : public EZ_IBasicRequestBuffer
{
   public : 
    RawAdapter(HttpRequest &, EZ_IBasicRawBuffer::RAWTYPE type);
    virtual bool                setData(const std::string&) = 0; 
    virtual bool                setLength(const int&) = 0;
    virtual const std::string&  getData(void) const = 0;
    virtual const int&          getLength(void) const = 0;
    virtual const EZ_IBasicRawBuffer::RAWTYPE&  getRawType(void) const = 0;
    virtual bool                                setRawType(const EZ_IBasicRawBuffer::RAWTYPE&) = 0;
    protected:
      HttpRequest 	&m_httpreq;
      EZ_IBasicRawBuffer::RAWTYPE		m_rawtype;
      int		m_rawlen;
};
#endif

