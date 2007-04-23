#ifndef __MODREQUESTADAPTER_H__
#define __MODREQUESTADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"

class ResponseAdapter : public EZ_IBasicResponseBuffer
{
  public:
    ResponseAdapter(HttpRequest &);

    virtual bool                setHTTPVersion(const std::string&);                     // returns true if the HTTP version was setted correctly, false otherwise... (see the FAQ for string format)
        virtual bool                setStatusCode(const CODE&);                             // returns true if the status code was setted correctly, false otherwise...
    virtual bool                setReasonPhrase(const std::string&);                    // returns true if the HTTP reason phrase was setted correctly, false otherwise...
    virtual bool                setHeader(const std::string&, const std::string&);      // returns true if the header (key, value) was added correctly, false otherwise...
    virtual bool                setHeaders(const MHEADERS&);                            // returns true if the headers map was added correctly, false otherwise...

    virtual bool                delHeader(const std::string&);                          // returns true if the wanted header was deleted, false otherwise

    virtual const std::string&  getHTTPVersion(void) const;                             // returns a const reference on the HTTP version...
    virtual const CODE&         getStatusCode(void) const;                              // returns a const reference on the HTTP status code...
    virtual const std::string&  getReasonPhrase(void) const;                            // returns a const reference on the HTTP reason phrase
    virtual const MHEADERS&     getHeaders(void) const;                                 // returns a const reference on the headers map
    virtual const std::string&  getHeader(const std::string&) const;                    // returns a const reference on the wanted header's value
    protected:
      HttpRequest 	&m_httpreq;
      MHEADERS		m_headers;
      CODE		m_code;
};
#endif

