
#include <iostream>
#include "mod_q_out.hpp"

ModQOut::~ModQOut()
{
}

bool                    ModQOut::needProceed()
{
  return true;
}

bool                    ModQOut::proceed()
{
  return true;
}

EZ_IModule&             ModQOut::getNext() const
{
  return (const_cast<ModQOut&>(*this));
}

bool                    ModQOut::setNext(EZ_IModule &)
{
  return true;
}

EZ_IBuffer              *ModQOut::popBuffer()
{
  EZ_IBuffer            *buf;

  if (m_bufs.size() > 0)
    {
      buf = *m_bufs.begin();
      m_bufs.pop_front();
      return buf;
    }
  else
    return 0;
}

bool                    ModQOut::pushBuffer(EZ_IBuffer &a_buf)
{
  m_bufs.push_back(&a_buf);
}

std::string             ModQOut::getModuleName() const
{
  return ("Zia Internal : ModQOut");
}

std::string             ModQOut::getModuleDescription() const
{
  return ("Zia Internal : ModQOut");
}

EZ_IModule::ModuleLevel ModQOut::getModuleLevel() const
{
  return (EZ_IModule::EZ_IN);
}

void                    ModQOut::setServerConfiguration(const std::string&, const std::string&)
{
}

void                    ModQOut::clean(void)
{
  for (std::list<EZ_IBuffer *>::iterator i = m_bufs.begin();
       i != m_bufs.end(); i++)
    delete *i;
  m_bufs.clear();
}

unsigned int            ModQOut::get_buffer_count()
{
  return m_bufs.size();
}
