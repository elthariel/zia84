//
// main.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 16:04:18 2007 Nahlwe
// Last update Tue Mar 13 05:57:25 2007 Nahlwe
//

#include <pthread.h>
#include "worker.hpp"
#include "main.hpp"

int main (int ac, char **av)
{
  WorkerPool    pool(10);

  pool.main_loop();
}
