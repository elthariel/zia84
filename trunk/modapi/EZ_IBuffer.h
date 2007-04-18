#ifndef __IBUFFER_H__
# define __IBUFFER_H__

class   EZ_IBuffer
{
    public:
        enum    TYPE                                                // Buffer type
        {
            EZ_RAW = 1,                                                 // type 'RAW BUFFER'
            EZ_REQUEST,                                                 // type 'REQUEST HEADER'
            EZ_RESPONSE                                                 // type 'RESPONSE HEADER'
        };
        virtual                         ~EZ_IBuffer(){}
        virtual	EZ_IBuffer::TYPE        getBufferType() const = 0;   // Returns the current buffer type

        // get the request id, for the module to be able to associate a buffer with the corresponding request.
        virtual const int&              getRequestID(void) const = 0;
        // set the request id. See above.
        virtual void                    setRequestID(const int&) = 0;
};

#endif /* __IBUFFER_H__ */
