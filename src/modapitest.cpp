
#include <iostream>
#include "module_loader.hpp"
#include "mod_q_out.hpp"
#include "mod_manager.hpp"

using namespace std;

int main (int ac, char **av)
{
  EZ_IModule *mod;

  mod = ModLoader::get().load("modtest/mod_test.so");
  cout << mod->getModuleDescription() << endl;
}
