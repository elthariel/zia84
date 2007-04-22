

class RequestAdapter : public EziBasicRequestBuffer
{
   public : 
    RequestAdapter(HttpRequest &);
        
    virtual bool                setMethode(const METHODE&) ; // returns true if the HTTP methode was setted correctly, false otherwise...
        virtual bool                setRequestURI(const std::string&) = 0; // returns true if the request URI was setted correctly, false otherwise...
        virtual bool                setHTTPVersion(const std::string&) = 0;  // returns true if the HTTP version was setted correctly, false otherwise... (see the FAQ for string format)
        virtual bool                setHeader(const std::string&, const std::string&) = 0;  // returns true if the header (key, value) was added correctly, false otherwise...
        virtual bool                setHeaders(const MHEADERS&) = 0;  // returns true if the headers map was added correctly, false otherwise...

        virtual bool                delHeader(const std::string&) = 0;                          // returns true if the wanted header was deleted, false otherwise

        virtual const METHODE&      getMethode(void) const = 0;                                 // returns a const reference on the HTTP methode
        virtual const std::string&  getRequestURI(void) const = 0;                              // returns a const reference on the request URI
        virtual const std::string&  getHTTPVersion(void) const = 0;                             // returns a const reference on the HTTP version
        virtual const MHEADERS&     getHeaders(void) const = 0;                                 // returns a const reference on the headers map
   private:

}
