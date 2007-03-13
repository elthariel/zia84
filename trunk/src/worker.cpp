//
// worker.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 12:18:15 2007 Nahlwe
// Last update Tue Mar 13 07:04:47 2007 Nahlwe
//

#include <iostream>
#include <pthread.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include "worker.hpp"
#include "help.hpp"


using namespace std;

/*
 * TaskList
 */

void            TaskList::put(TaskList::Task &t)
{
  m_mutex.lock();
  m_tasks.push_back(t);
  m_mutex.unlock();
  m_event.wake_one();
}

// FIXME wait could wait more than one thread ? so we could
// get some bugs here.
TaskList::Task  TaskList::get()
{
  m_mutex.lock();
  if (m_tasks.empty())
    {
      m_mutex.unlock();
      m_event.wait();
      m_mutex.lock();
      TaskList::Task tmp = *(m_tasks.begin());
      m_tasks.pop_front();
      m_mutex.unlock();
      return (tmp);
    }
  else
    {
      TaskList::Task tmp = *m_tasks.begin();
      m_tasks.pop_front();
      m_mutex.unlock();
      return (tmp);
    }
}


/*
 * Worker class
 */
Worker::~Worker()
{
}

Worker::Worker(TaskList &a_tasks)
  : m_tasks(a_tasks),
    m_thread(*this)
{
}

void                    Worker::operator()()
{
  cerr << "A worker was created" << endl;
  main_loop();
}

void                    Worker::main_loop()
{
  while(42)
    {
      TaskList::Task    t = m_tasks.get();
      switch(t.type)
        {
        case TaskList::TaskRequest:
          request_entry(*t.content.sock);
          delete t.content.sock;
          break;
        case TaskList::TaskKillWorker:
        case TaskList::TaskTypeCount:
          cout << "Killing a worker" << endl;
          exit(t.type);
          break;
        }
    }
}

void                    Worker::request_entry(Socket &a_socket)
{
  // FIXME process the request.
  cout << "A worker received a request" << endl;
}



/*
 * WorkerPool class,
 */
WorkerPool::WorkerPool(unsigned int a_worker_count)
  : m_worker_count(a_worker_count)
{
  unsigned int  i;
  Worker        *tmp;

  sockaddr_in           addr;
  hostent               *host;

  //FIXME Should use HttpConf singleton to get
  // ip, port, etc

  m_main_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (m_main_socket == -1)
    cerr << "Unable to create socket" << strerror(errno) << endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  host = gethostbyname("192.168.0.42");
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind (m_main_socket, (sockaddr *)&addr,
            sizeof(addr)) != 0)
    cerr << "Unable to bind main socket : " << strerror(errno)
         << endl;
  if (listen(m_main_socket, 30) == -1)
    cerr << "Unable to enter listening state : " << strerror(errno)
         << endl;


  /*
   * Creating workers
   */
  for (i = 0; i < a_worker_count; i++)
    {
      tmp = new Worker(m_tasks);
      m_workers.push_back(tmp);
    }
}

void                    WorkerPool::main_loop()
{
  sockaddr              addr;
  socklen_t             addr_len;
  int                   new_fd;

  Socket                *tmp;

  cerr << "Zia server entering accept connection mode." << endl;
  while (42)
    {
      new_fd = accept(m_main_socket, &addr, &addr_len);
      cerr << "Received connection" << endl;
      if (new_fd < 0)
        cerr << "Error on accepting the connection : "
             << strerror(errno) << endl;
      else
        {
          TaskList::Task t;
          tmp = new Socket(new_fd);
          t.type = TaskList::TaskRequest;
          t.content.sock = tmp;
          m_tasks.put(t);
        }
    }
}
