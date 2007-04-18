#ifndef __IBASICSTREAMBUFFER_H__
# define __IBASICSTREAMBUFFER_H__

#include <map>
#include <string>
#include <EZ_IBuffer.h>

typedef std::map <std::string, std::string>     MHEADERS;                                   // shorter to write many times... (key, value)
typedef int                                     CODE;                                       // more explicit...

class       EZ_IBasicRequestBuffer :   public EZ_IBuffer
{
    public:
        enum    METHODE                                                                         // http methodes
        {
            OPTIONS = 1, GET, HEAD, POST, PUT, DEL, TRACE, CONNECT
        };
        virtual                     ~EZ_IBasicRequestBuffer(){}
        virtual bool                setMethode(const METHODE&) = 0;                             // returns true if the HTTP methode was setted correctly, false otherwise...
        virtual bool                setRequestURI(const std::string&) = 0;                      // returns true if the request URI was setted correctly, false otherwise...
        virtual bool                setHTTPVersion(const std::string&) = 0;                     // returns true if the HTTP version was setted correctly, false otherwise... (see the FAQ for string format)
        virtual bool                setHeader(const std::string&, const std::string&) = 0;      // returns true if the header (key, value) was added correctly, false otherwise...
        virtual bool                setHeaders(const MHEADERS&) = 0;                                // returns true if the headers map was added correctly, false otherwise...

        virtual bool                delHeader(const std::string&) = 0;                          // returns true if the wanted header was deleted, false otherwise

        virtual const METHODE&      getMethode(void) const = 0;                                 // returns a const reference on the HTTP methode
        virtual const std::string&  getRequestURI(void) const = 0;                              // returns a const reference on the request URI
        virtual const std::string&  getHTTPVersion(void) const = 0;                             // returns a const reference on the HTTP version
        virtual const MHEADERS&     getHeaders(void) const = 0;                                 // returns a const reference on the headers map
        virtual const std::string&  getHeader(const std::string&) const = 0;                    // returns a const reference on the wanted header's value
};

class	EZ_IBasicResponseBuffer    :   public EZ_IBuffer
{
    public:
        virtual                     ~EZ_IBasicResponseBuffer(){}
        virtual bool                setHTTPVersion(const std::string&) = 0;                     // returns true if the HTTP version was setted correctly, false otherwise... (see the FAQ for string format)
        virtual bool                setStatusCode(const CODE&) = 0;                             // returns true if the status code was setted correctly, false otherwise...
        virtual bool                setReasonPhrase(const std::string&) = 0;                    // returns true if the HTTP reason phrase was setted correctly, false otherwise...
        virtual bool                setHeader(const std::string&, const std::string&) = 0;      // returns true if the header (key, value) was added correctly, false otherwise...
        virtual bool                setHeaders(const MHEADERS&) = 0;                            // returns true if the headers map was added correctly, false otherwise...

        virtual bool                delHeader(const std::string&) = 0;                          // returns true if the wanted header was deleted, false otherwise

        virtual const std::string&  getHTTPVersion(void) const = 0;                             // returns a const reference on the HTTP version...
        virtual const CODE&         getStatusCode(void) const = 0;                              // returns a const reference on the HTTP status code...
        virtual const std::string&  getReasonPhrase(void) const = 0;                            // returns a const reference on the HTTP reason phrase
        virtual const MHEADERS&     getHeaders(void) const = 0;                                 // returns a const reference on the headers map
        virtual const std::string&  getHeader(const std::string&) const = 0;                    // returns a const reference on the wanted header's value
};

class	EZ_IBasicRawBuffer         :   public EZ_IBuffer
{
    public:
        enum    RAWTYPE
        {
            REQUEST = 1, RESPONSE
        };
        virtual                     ~EZ_IBasicRawBuffer(){}
        virtual bool                setData(const std::string&) = 0;                            // returns true if the data was setted correctly, false otherwise...
        virtual bool                setLength(const int&) = 0;                                  // returns true if the data length was setted correctly, false otherwise... (for compliance...)

        virtual const std::string&  getData(void) const = 0;                                    // returns a const reference on the data...
        virtual const int&          getLength(void) const = 0;                                  // returns a const reference on the data's length... (for compliance...)

        virtual const EZ_IBasicRawBuffer::RAWTYPE&  getRawType(void) const = 0;                 // get the RAW buffer type. REQUEST or RESPONSE.
        virtual bool                                setRawType(const EZ_IBasicRawBuffer::RAWTYPE&) = 0;     // set the RAW buffer type. REQUEST or RESPONSE.
};

#endif /* __IBASICSTREAMBUFFER_H__ */
