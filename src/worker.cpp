//
// worker.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 12:18:15 2007 Nahlwe
// Last update Mon Apr 23 12:32:13 2007 
//

#ifdef XNIX
# include <netinet/ip.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
#endif
#include <iostream>
#include <pthread.h>
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

  m_mutex.lock();
  m_tasks.push_back(t);
  count = m_tasks.size();
  m_mutex.unlock();
  m_event.wake_one();
  return (count);
}

bool            TaskList::get(TaskList::Task *out_task)
{
  m_mutex.lock();
  if (m_tasks.empty())
    {
      m_mutex.unlock();
      m_event.wait(HttpdConf::get().get_key_int("/zia/intern/wait"));
      //m_event.wait();
      m_mutex.lock();
      if (!m_tasks.empty())
        {
          *out_task = *(m_tasks.begin());
          m_tasks.pop_front();
          m_mutex.unlock();
          return (true);
        }
      else
        {
          m_mutex.unlock();
          return false;
        }
    }
  else
    {
      *out_task = *m_tasks.begin();
      m_tasks.pop_front();
      m_mutex.unlock();
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
            {
              cerr << "A worker commits suicide" << endl;
              delete this;
              return;
            }
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
  string2	header;
  string	body;
  string	file;
  string 	request;
 
  if (httpreq.HttpCheckRequest())
  {
    try {
         if (!httpreq.m_http_map["method"].compare("GET") || !httpreq.m_http_map["method"].compare("POST"))
           if (httpreq.reqcgi)
              body = httpreq.HttpGetCGI();
	 if (httpreq.reqdir)
	   body = httpreq.HttpGetDir();
         request += httpreq.HttpGetStatus();
         request +=  httpreq.HttpCreateHeader();
         if (!httpreq.m_http_map["method"].compare("GET") || !httpreq.m_http_map["method"].compare("POST"))
         {
           if (httpreq.reqcgi)
	     request +=  body;
           if (httpreq.reqfile)
             request += httpreq.HttpGetFile();
	   if (httpreq.reqdir)
	     request += body;
         }
	 cout << "request" << endl << request << endl << "---emd of req" << endl;
	 a_socket << request;
      }
     catch (SocketError*)
    {
   	cout << "error socket" << endl;
    }
  }
}

/*
 * WorkerPool class,
 */
WorkerPool::WorkerPool(unsigned int a_worker_count)
  : m_worker_count(a_worker_count)
{
  unsigned int  i;
  Worker        *tmp;

#ifdef XNIX
  sockaddr_in           addr;
  hostent               *host;

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
#endif

#ifdef WIN_32
  int error;
  WSADATA wsaData;
  WORD version;
  sockaddr_in sin;

  version = MAKEWORD( 2, 0 );
  error = WSAStartup(version, &wsaData);
  if (error != 0)
    cerr << "Unable to initialize Winsock subsystem" << endl;
  m_main_socket = socket(AF_INET, SOCK_STREAM, 0);
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(HttpdConf::get().get_key_int("/zia/network/port"));
  if (bind(m_main_socket, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
    cerr << "Unable to bind socket" << endl;
  if (listen(m_main_socket, 30) == SOCKET_ERROR)
    cerr << "Unable to enter listening state for main socket" << endl;
#endif

  /*
   * Creating workers
   */

  for (i = 0; i < a_worker_count; i++)
    {
      tmp = new Worker(m_tasks, true); // true = Highlanders.
    }
}

void                    WorkerPool::main_loop()
{
  Socket                *tmp;

  cerr << "Zia server entering accept connection mode." << endl;
  while (42)
    {
#ifdef XNIX
      sockaddr              addr;
      socklen_t             addr_len;
      int                   new_fd;

      memset(&addr, 0, sizeof(addr));
      memset(&addr_len, 0, sizeof(int));
      new_fd = accept(m_main_socket, &addr, &addr_len);
      cerr << "Received connection" << endl;
      if (new_fd < 0)
        cerr << "Error on accepting the connection : " << new_fd << " : "
             << strerror(errno) << endl;
      else
        {
#endif
#ifdef WIN_32
          struct sockaddr sin;
          SOCKET new_fd;
          int length;

          length = sizeof sin;
          new_fd = accept(m_main_socket, &sin, &length);
          if (new_fd == INVALID_SOCKET)
            cerr << "Unable to accept client : " << WSAGetLastError() << endl;
          else
            {
              cerr << "Received a connection" << endl;
#endif
          TaskList::Task t;
          tmp = new Socket(new_fd);
          t.type = TaskList::TaskRequest;
          t.content.sock = tmp;
          if (m_tasks.put(t) >
              HttpdConf::get().get_key_int("/zia/intern/thresold"))
            {
              new Worker(m_tasks, false);
            }
        }
    }
}
