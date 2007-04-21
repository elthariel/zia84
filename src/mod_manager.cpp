/*
 * Module manager
 */

#include "mod_manager.hpp"

ModManager::ModManager()
 : m_in(m_in_out),
   m_proceed(m_proceed_out),
   m_out(m_out_out)
{
}

void            ModManager::push_module(EZ_IModule &a_mod)
{
  EZ_IModule    **modq;

  switch (a_mod.getModuleLevel())
    {
    case EZ_IModule::EZ_IN :
      modq = &m_in;
      break;
    case EZ_IModule::EZ_PROCEED :
      modq = &m_proceed;
      break;
    case EZ_IModule::EZ_OUT :
      modq = &m_out;
      break;
    default:
      return;
    }
  a_mod.setNext(**modq);
  *modq = &a_mod;
}

void          ModManager::push_buffer(EZ_IBuffer &a_buf,
                          EZ_IModule::ModuleLevel a_buf_lvl)
{
  EZ_IModule  *modq;

  switch (a_buf_lvl)
    {
    case EZ_IModule::EZ_IN :
      modq = m_in;
      break;
    case EZ_IModule::EZ_PROCEED :
      modq = m_proceed;
      break;
    case EZ_IModule::EZ_OUT :
      modq = m_out
        break;
    default:
      return;
    }
  modq->pushBuffer(a_buf);
}

unsigned int  ModManager::get_buffer_count(EZ_IModule::ModuleLevel a_buf_lvl)
{
  ModQOut *qout;

  switch (a_buf_lvl)
    {
    case EZ_IModule::EZ_IN :
      qout = &m_in_out;
      break;
    case EZ_IModule::EZ_PROCEED :
      qout = &m_proceed_out;
      break;
    case EZ_IModule::EZ_OUT :
      qout = &m_out_out;
        break;
    default:
      return;
    }
  return (qout->get_buffer_count());
}

EZ_IBuffer    *ModManager::pop_buffer(EZ_IModule::ModuleLevel a_buf_lvl)
{
  ModQOut *qout;

  switch (a_buf_lvl)
    {
    case EZ_IModule::EZ_IN :
      qout = &m_in_out;
      break;
    case EZ_IModule::EZ_PROCEED :
      qout = &m_proceed_out;
      break;
    case EZ_IModule::EZ_OUT :
      qout = &m_out_out;
        break;
    default:
      return;
    }
  return qout->popBuffer();
}

bool          ModManager::process_stack(EZ_IModule::ModuleLevel a_buf_lvl)
{
  EZ_IModule  *modq;

  switch (a_buf_lvl)
    {
    case EZ_IModule::EZ_IN :
      modq = m_in;
      break;
    case EZ_IModule::EZ_PROCEED :
      modq = m_proceed;
      break;
    case EZ_IModule::EZ_OUT :
      modq = m_out
        break;
    default:
      return;
    }
  _process_stack(modq);
  return (get_buffer_count(a_buf_lvl) > 0);
}

bool          ModManager::_process_stack(EZ_IModule *a_stack)
{
  EZ_IModule *mod = a_stack;

  while(mod != &(mod->getNext()))
    {
      if (mod->needProceed())
        mod->proceed();
      mod = &(mod->getNext());
    }
}
