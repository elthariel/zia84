//
// main.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 16:04:18 2007 Nahlwe
// Last update Fri Mar 16 11:31:01 2007 Nahlwe
//

#include <pthread.h>
#include "conf.hpp"
#include "worker.hpp"
#include "main.hpp"
#include "http_request.hpp"

int main (int ac, char **av)
{
  WorkerPool    *pool;

  if (ac > 1)
    HttpdConf::init(av[1]);

  pool = new WorkerPool(HttpdConf::get()
                        .get_key_int("/zia/intern/workers"));
  pool->main_loop();
  delete pool;
}
