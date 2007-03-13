//
// worker.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 12:18:15 2007 Nahlwe
// Last update Mon Mar 12 19:28:10 2007 Nahlwe
//

#include <iostream>
#include <pthread.h>
#include "worker.hpp"

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
TaskList::Task  &TaskList::get()
{
  m_mutex.lock();
  if (m_tasks.empty())
    {
      m_mutex.unlock();
      m_event.wait();
      m_mutex.lock();
      TaskList::Task &tmp = m_tasks.begin();
      m_tasks.pop_front();
      m_mutex.unlock();
      return (tmp);
    }
  else
    {
      TaskList::Task &tmp = m_tasks.begin();
      m_tasks.pop_front();
      m_mutex.unlock();
      return (tmp);
    }
}


/*
 * Worker class
 */
Worker::Worker(TaskList &a_tasks)
  : m_tasks(a_tasks),
    m_thread(*this)
{
}

void                    Worker::main_loop()
{
  while(42)
    {
      TaskList::Task    &t = m_tasks.get();
      switch(t.type)
        {
        case TaskRequest:
          request_entry(t.content.sock);
          delete *t.content.sock;
          break;
        case: TaskKillWorker:
        case: TaskTypeCount:
          cout << "Killing a worker" << endl;
          exit(t.type);
          break;
        }
      delete *t;
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
}

void                    WorkerPool::main_loop();
