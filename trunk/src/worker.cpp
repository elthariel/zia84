//
// worker.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 12:18:15 2007 Nahlwe
// Last update Fri Mar 16 11:32:46 2007 Nahlwe
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
#include "conf.hpp"

using namespace std;

/*
 * TaskList
 */

bool            TaskList::put(TaskList::Task &t)
{
  unsigned int  count;

  m_tasks.push_back(t);
  count = m_tasks.size();
  m_event.wake_one();
  return (1);
}

bool            TaskList::get(TaskList::Task *out_task)
{
  if (m_tasks.empty())
    {
      m_event.wait(HttpdConf::get().get_key_int("/zia/intern/wait"));
      m_mutex.lock();
      if (!m_tasks.empty())
        {
          *out_task = *(m_tasks.begin());
          m_tasks.pop_front();
          m_mutex.unlock();
          return (true);
        }
      else
        return false;
    }
  else
    {
      *out_task = *m_tasks.begin();
      m_tasks.pop_front();
      return (true);
    }
}


/*
 * Worker class
 */
Worker::~Worker()
{
}

Worker::Worker(TaskList &a_tasks, bool a_highlander)
  : m_tasks(a_tasks),
    m_highlander(a_highlander),
    m_thread(*this)
{
}

void                    Worker::operator()()
{
  main_loop();
}

void                    Worker::main_loop()
{
  while(42)
    {
      TaskList::Task    t;
      while (!m_tasks.get(&t))
        {
          if (!m_highlander)
            delete this;
        }
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
 HttpRequest	httpreq(a_socket);

 if (!httpreq.m_http_map["method"].compare("GET"))
 {
   FilePath	file;
  
   httpreq.HttpFile(file);
/*   std::string chunk;
   string2  chunk2;
    chunk = httpreq.m_http_map[httpreq.m_http_map["method"]]; 
  chunk2.append(chunk);
  chunk2.split(" ", chunk);
    file.Path += chunk;
    //mettre le path parser xml au lieu de sa
   if (!file.Path.compare("../www/"))
    file.Path = "../www/index.html";
  */  
    a_socket << file; 
 }

}



/*
 * WorkerPool class,
 */
WorkerPool::WorkerPool(unsigned int a_worker_count)
  : m_worker_count(a_worker_count)
{
  //unsigned int  i;
  //Worker        *tmp;

  sockaddr_in           addr;
  hostent               *host;

  //FIXME Should use HttpConf singleton to get
  // ip, port, etc

  m_main_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (m_main_socket == -1)
    cerr << "Unable to create socket" << strerror(errno) << endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(HttpdConf::get().get_key_int("/zia/network/port"));
  host = gethostbyname("127.0.0.1");
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
  /* 
  for (i = 0; i < a_worker_count; i++)
    {
      tmp = new Worker(m_tasks, true); // true = Highlanders.
    }*/
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
      memset(&addr, 0, sizeof(addr));
      memset(&addr_len, 0, sizeof(int));
      new_fd = accept(m_main_socket, &addr, &addr_len);
      cerr << "Received connection" << endl;
      if (new_fd < 0)
        cerr << "Error on accepting the connection : " << new_fd << " : "
             << strerror(errno) << endl;
      else
        {
          TaskList::Task t;
          tmp = new Socket(new_fd);
          t.type = TaskList::TaskRequest;
          t.content.sock = tmp;
          if (m_tasks.put(t))
	  {
            new Worker(m_tasks, false);
	  }
        }
    }
}
