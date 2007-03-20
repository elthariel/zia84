//
// worker.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 12:18:17 2007 Nahlwe
// Last update Fri Mar 16 07:22:11 2007 Nahlwe
//

#ifndef WORKER_HPP_
# define WORKER_HPP_

#include <list>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "thread.hpp"
#include "socket.hpp"
#include "http_request.hpp"

class TaskList
{
public:
  enum TaskType
    {
      TaskRequest,
      TaskKillWorker,
      TaskTypeCount,
    };
  struct Task
  {
    TaskType    type;
    union
    {
      Socket    *sock;
    } content;
  };

  /**
   * put() pushes back a taks on the task list. It wakes a worker
   * if there is any waiting for a task. Return true if a new worker
   * is needed.
   */
  bool                  put(Task &);

  /**
   * Get a task. It blocks the calling process if there isn't any
   * task until the main process puts enought tasks to wake it
   * (depending on the number of inactive workers).
   * Return true if a task was obtained.
   */
  bool                  get(Task *);

private:
  std::list<Task>       m_tasks;
  Mutex                 m_mutex;
  Event                 m_event;
};

class Worker : public iFoncteur0<void>, public NonCopyable
{
public:
  Worker(TaskList &a_tasks, bool a_highlander = false);
  ~Worker();

  void                  operator()();

private:
  TaskList              &m_tasks;
  bool                  m_highlander;
  Thread                m_thread;

  void                  main_loop();
  void                  request_entry(Socket &a_socket);
};

class WorkerPool : public NonCopyable
{
public:
  WorkerPool(unsigned int a_worker_count);

  void                  main_loop();

private:
  WorkerPool();

  unsigned int          m_worker_count;
  TaskList              m_tasks;
  int                   m_main_socket;
};

#endif
