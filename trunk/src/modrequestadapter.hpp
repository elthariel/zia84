#ifndef __MODREQUESTADAPTER_H__
#define __MODREQUESTADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"
#include "my_uuid.hpp"

class RequestAdapter : public EZ_IBasicRequestBuffer
{
public :
  RequestAdapter(HttpRequest &, int );



  const METHODE RequestAdapterGetMethode(std::string &methode);
  virtual bool                setMethode(const METHODE&) ;
  virtual bool                setRequestURI(const std::string&);
  virtual bool                setHTTPVersion(const std::string&);
  virtual bool                setHeader(const std::string&, const std::string&);
  virtual bool                setHeaders(const MHEADERS&);
  virtual bool                delHeader(const std::string&);
  virtual const METHODE&      getMethode(void) const;
  virtual const std::string&  getRequestURI(void) const;
  virtual const std::string&  getHTTPVersion(void) const;
  virtual const MHEADERS&     getHeaders(void) const;
  virtual const std::string&  getHeader(const std::string&) const;
  virtual const int&	      getRequestID(void) const;
  virtual void 	              setRequestID(const int &);
  virtual EZ_IBuffer::TYPE    getBufferType() const;
protected:
  HttpRequest 	&m_httpreq;
  METHODE		m_httpmethod;
  MHEADERS		m_headers;
  int		m_id;
};
#endif
