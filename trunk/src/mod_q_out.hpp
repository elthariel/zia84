/*
 * ModQOut, end of each level module list
 */

#ifndef MODQOUT_HPP_
# define MODQOUT_HPP_

#include <list>
#include <EZ_IModule.h>

class ModQOut : public EZ_IModule
{
public:
  virtual                         ~ModQOut();

  virtual bool                    needProceed();
  virtual bool                    proceed();

  virtual EZ_IModule&             getNext() const;
  virtual bool                    setNext(EZ_IModule &);

  virtual EZ_IBuffer              *popBuffer();
  virtual bool                    pushBuffer(EZ_IBuffer &);

  virtual std::string             getModuleName() const;
  virtual std::string             getModuleDescription() const;
  virtual EZ_IModule::ModuleLevel getModuleLevel() const;

  virtual void                    setServerConfiguration(const std::string&, const std::string&);

  virtual void                    clean(void);

  unsigned int                    get_buffer_count();
protected:
  std::list<EZ_IBuffer *>         m_bufs;
};

#endif
