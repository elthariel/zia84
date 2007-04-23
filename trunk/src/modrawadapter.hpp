#ifndef __MODRAWADAPTER_H__
#define __MODRAWADAPTER_H__
#include "http_request.hpp"
#include "EZ_IStreamBuffer.h"


class RawAdapter : public EZ_IBasicRequestBuffer
{
   public : 
    RawAdapter(HttpRequest &);
    virtual bool                setData(const std::string&) = 0;                            // returns true if the data was setted correctly, false otherwise...
    virtual bool                setLength(const int&) = 0;                                  // returns true if the data length was setted correctly, false otherwise... (for compliance...)

    virtual const std::string&  getData(void) const = 0;                                    // returns a const reference on the data...
    virtual const int&          getLength(void) const = 0;                                  // returns a const reference on the data's length... (for compliance...)

    virtual const EZ_IBasicRawBuffer::RAWTYPE&  getRawType(void) const = 0;                 // get the RAW buffer type. REQUEST or RESPONSE.
    virtual bool                                setRawType(const EZ_IBasicRawBuffer::RAWTYPE&) = 0;     // set the RAW buffer type. REQUEST or RESPONSE.

    protected:
      HttpRequest 	&m_httpreq;
      EZ_IBasicRawBuffer::RAWTYPE		m_rawtype;
      int		m_rawlen;
};
#endif

