/*
 * ModPsp
 */

#ifndef __MOD_PSP_HPP_
# define __MOD_PSP_HPP_

# include <EZ_IModule.h>
# include <EZ_IStreamBuffer.h>
# include <list>
# include <string>

/*
 * Type of buffer. :
 *   - in  : RequestBuffer
 *   - out : ResponseBuffer
 *   - raw : RawBuffer
 */
enum BufferType
  {
    IN, RAW, OUT, NONE
  };

/**
 * return the type of a_buf
 */
BufferType      identify_buffer(EZ_IBuffer *a_buf);

struct bundle
{
  bundle();
  EZ_IBasicRequestBuffer        *request;
  EZ_IBasicRawBuffer            *raw_post;
  EZ_IBasicResponseBuffer       *response;
  EZ_IBasicRawBuffer            *raw_response;
};

class ModPsp : public EZ_IModule
{
public:
  typedef std::list<EZ_IBuffer *> buflist;

  ModPsp();

  virtual bool                    needProceed();
  virtual bool                    proceed();

  virtual const EZ_IModule&       getNext() const;
  virtual bool                    setNext(EZ_IModule &);

  virtual EZ_IBuffer              *popBuffer();
  virtual bool                    pushBuffer(EZ_IBuffer &);

  virtual std::string             getModuleName() const;
  virtual std::string             getModuleDescription() const;
  virtual EZ_IModule::ModuleLevel getModuleLevel() const;

  virtual void                    setServerConfiguration(const std::string&, const std::string&);

  virtual void                    clean(void);

protected:
  char                          **make_env(EZ_IBasicRequestBuffer *);
  bool                          have_buffer_bundle(int *);
  bool                          have_buffer_bundle_id(int a_req_id);
  bundle                        make_bundle(int a_req_id);

  buflist                       m_bufs;
  EZ_IModule                    *m_next_mod;
};

#endif /* __MOD_PSP_HPP_ */
