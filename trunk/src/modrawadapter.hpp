#ifndef __MODRAWADAPTER_H__
#define __MODRAWADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"


class RawAdapter : public EZ_IBasicRawBuffer
{
public :
  RawAdapter(HttpRequest &,int,  EZ_IBasicRawBuffer::RAWTYPE);
  virtual bool                setData(const std::string&);
  virtual bool                setLength(const int&);
  virtual const std::string&  getData(void) const;
  virtual const int&          getLength(void) const;
  virtual const EZ_IBasicRawBuffer::RAWTYPE&  getRawType(void) const;
  virtual bool                                setRawType(const EZ_IBasicRawBuffer::RAWTYPE&);

  virtual const int&          getRequestID(void) const;
  virtual void                setRequestID(const int &);
  virtual EZ_IBuffer::TYPE    getBufferType() const;
  /*  virtual bool                setRawType(const EZ_IBasicRawBuffer::RAWTYPE&);
      virtual const EZ_IBasicRawBuffer::RAWTYPE&  getRawType(void) const;*/
protected:
  HttpRequest 	&m_httpreq;
  int		m_id;
  EZ_IBasicRawBuffer::RAWTYPE		m_rawtype;
  int		m_rawlen;
};
#endif

