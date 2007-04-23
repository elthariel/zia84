/*
 * Module manager
 */

#ifndef MOD_MANAGER_HPP_
# define MOD_MANAGER_HPP_

#include <EZ_IModule.h>
#include "mod_q_out.hpp"

class ModManager
{
public:
  ModManager();
  void          push_module(EZ_IModule &a_mod);
  void          push_buffer(EZ_IBuffer &a_buf,
                            EZ_IModule::ModuleLevel a_buf_lvl);
  unsigned int  get_buffer_count(EZ_IModule::ModuleLevel a_buf_lvl);
  EZ_IBuffer    *pop_buffer(EZ_IModule::ModuleLevel a_buf_lvl);
  bool          process_stack(EZ_IModule::ModuleLevel a_buf_lvl);
protected:
  void          _process_stack(EZ_IModule *a_stack);

  ModQOut       m_in_out, m_proceed_out, m_out_out;
  EZ_IModule    *m_in, *m_proceed, *m_out;
};

#endif
