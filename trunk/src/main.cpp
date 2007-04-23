//
// main.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 16:04:18 2007 Nahlwe
// Last update Mon Apr 23 10:22:17 2007 
//

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "conf.hpp"
#include "worker.hpp"
#include "main.hpp"
#include "http_request.hpp"

#define PID_PATH        "/tmp/zia.pid"

using namespace std;

#ifdef XNIX
static int      pid_file()
{
  int           pid_fd;
  char          str[16];

  pid_fd = open(PID_PATH, O_RDWR | O_CREAT, 0640);
  if (pid_fd < 0)
    {
      cerr << "Unable to open /tmp/zia.pid (is another instance running ?)"
           << endl;
      exit (3);
    }
  if (lockf(pid_fd, F_TLOCK, 0) < 0)
    {
      cerr << "Unable to lock /tmp/zia.pid (is another instance running ?)"
           << endl;
      exit (3);
    }

  sprintf(str, "%d\n", getpid());
  write(pid_fd, str, strlen(str));
  return pid_fd;
}

static int      kill_pid_file(int pid_fd)
{
  //  lockf(pid_fd, F_ULOCK, 0);
  close(pid_fd);
  unlink("/tmp/zia.pid");
}

static int      daemonize()
{
  int           i;
  int           pid_fd;

  // Independant process
  i = fork();
  if (i < 0)
    exit(1);
	if (i > 0)
    exit(0);

  // pid file
  pid_fd = pid_file();
  cout << "pid:" << getpid() << endl;

  // Detach from the terminal
  if (setsid() < 0)
    exit(2);

  // Detach file descriptors ?

  // Restrict file creation ?
  // umask(027);

  // Chdir into www_root if not chrooted

  // Signal masking and handling

  return pid_fd;
}
#endif

int main (int ac, char **av)
{
  WorkerPool    *pool;
  int           pid_fd;

  if (ac < 2)
  {
    cout <<  av[0]  << " : You must specify a config file" << endl;
    return (1);
  }
  HttpdConf::init(av[1]);

#ifdef XNIX
  signal(SIGPIPE, SIG_IGN);
  //pid_fd = daemonize();
#endif

  pool = new WorkerPool(HttpdConf::get()
                        .get_key_int("/zia/intern/workers"));
  pool->main_loop();
  delete pool;

#ifdef XNIX
  kill_pid_file(0);
#endif
}
