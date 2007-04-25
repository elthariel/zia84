#ifndef __MODRESPONSEADAPTER_H__
#define __MODRESPONSEADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"

class ResponseAdapter : public EZ_IBasicResponseBuffer
{
public:
  ResponseAdapter(HttpRequest &, int);

  virtual bool                setHTTPVersion(const std::string&);
  virtual bool                setStatusCode(const CODE&);
  virtual bool                setReasonPhrase(const std::string&);
  virtual bool                setHeader(const std::string&, const std::string&);
  virtual bool                setHeaders(const MHEADERS&);
  virtual bool                delHeader(const std::string&);
  virtual const std::string&  getHTTPVersion(void) const;
  virtual const CODE&         getStatusCode(void) const;
  virtual const std::string&  getReasonPhrase(void) const;
  virtual const MHEADERS&     getHeaders(void) const;
  virtual const std::string&  getHeader(const std::string&) const;
  virtual const int&	 	    getRequestID(void) const;
  virtual void 	    setRequestID(const int &);
  virtual EZ_IBuffer::TYPE    getBufferType() const;
protected:
  CODE		m_code;
  HttpRequest 	&m_httpreq;
  MHEADERS		m_headers;
  int		m_id;
};
#endif

