
#include <iostream>
#include "dlfcn.h"
#include "module_loader.hpp"

using namespace std;

ModLoader::ModLoader()
{
}

ModLoader::~ModLoader()
{
}

ModLoader       &ModLoader::get()
{
  if (m_instance != 0)
    m_instance = new ModLoader();
  return *m_instance;
}

EZ_IModule      *ModLoader::load(std::string path)
{
  void          *dll;
  void          *entry_point;
  char          *err_test_bak, err_test;


  dll = dlopen(path.c_str(), RTLD_LAZY);
  if (dll == NULL)
    {
      cerr << dlerror() << endl;
      return 0;
    }
  dlerror();
  err_test_bak = dlerror();
  entry_point = dlsym(dll, "getInstance");
  err_test = dlerror();
  if (err_test != err_test_bak)
    {
      cerr << err_test << endl;
      return (0);
    }
  return (((ModEntry)entry_point)());
}

