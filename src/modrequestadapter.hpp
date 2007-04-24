#ifndef __MODREQUESTADAPTER_H__
#define __MODREQUESTADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"


class RequestAdapter : public EZ_IBasicRequestBuffer
{
   public : 
    RequestAdapter(HttpRequest &);
        
	

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
	virtual const std::string&   getHeader(const std::string&) const;
    protected:
      HttpRequest 	&m_httpreq;
      METHODE		m_httpmethod;
      MHEADERS		m_headers;
};
#endif
