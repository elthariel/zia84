
#include <iostream>
#ifdef XNIX
# include "dlfcn.h"
#endif
#ifdef WIN_32
# include <windows.h>
#endif
#include "module_loader.hpp"

using namespace std;

ModLoader       *ModLoader::m_instance = 0;

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
#ifdef XNIX
  void          *dll;
  void          *entry_point;
  char          *err_test_bak, *err_test;


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
#endif
#ifdef WIN_32
  HMODULE       dll;
  FARPROC       entry_point;

  dll = LoadLibrary(path.c_str());
  if (dll == NULL)
    {
      cerr << "Unable to open " << path.c_str() << " : " << GetLastError() << endl;
      return (0);
    }
  entry_point = GetProcAddress(dll, "getInstance");
  if (entry_point == NULL)
    {
      cerr << "Unable to get module_entry_point address, is this a regular EvilZia module ? " << GetLastError() << endl;
      return (0);
    }
#endif

  return (((ModEntry)entry_point)());
}

