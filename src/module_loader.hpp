/*
 * Module loader
 */

#ifndef MOD_LOADER_HPP
# define MOD_LOADER_HPP

#include <string>
#include <EZ_IModule.h>

typedef EZ_IModule *(*ModEntry)();

class ModLoader
{
public:
  static ModLoader      &get();
  EZ_IModule            *load(std::string path);
protected:
  ModLoader();
  ~ModLoader();

  static ModLoader      *m_instance;

  //  std::string           m_mod_path;
};

#endif
