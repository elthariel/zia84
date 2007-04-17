//
// thread.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 08:04:50 2007 Nahlwe
// Last update Tue Apr 17 00:49:02 2007 Nahlwe
//

#ifndef THREAD_HPP_
# define THREAD_HPP_

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "foncteurs.hpp"
#include "help.hpp"

void            *run_thread(void *arg);

/** Thread class, inherit to create your own thread
 *
 */
class Thread : public NonCopyable
{
public:
  Thread(iFoncteur0<void> &);
  ~Thread();

  void                  join();
  bool                  operator==(Thread &a_th);
  bool                  operator!=(Thread &a_th);

  static void           sleep(unsigned int a_usecs);
  static void           yield();

private:
  Thread();
  void                  thread_func();

  pthread_t             m_thread;
  pthread_attr_t        m_thread_attr;
  iFoncteur0<void>      &m_fonc;
};

class Mutex : public NonCopyable
{
public:
  Mutex();
  ~Mutex();

  void                  lock();
  void                  unlock();
  int                   trylock();

protected:
  pthread_mutex_t       m_mutex;
};

class Event : protected Mutex
{
public:
  Event();
  ~Event();

  int                   wake_all();
  int                   wake_one();
  void                  wait();
  void                  wait(unsigned int a_sec);

private:
  pthread_cond_t        m_event;
};

class Semaphore
{
public:
  Semaphore(unsigned int a_init_value = 0);
  ~Semaphore();

  Semaphore     &operator++(); //Non-blocking.
  Semaphore     &operator--(); //Blocking.
  Semaphore     &operator+=(unsigned int a_added); //Non-blocking.
  Semaphore     &operator-=(unsigned int a_removed); //Blocking.
  unsigned int  get_value();

private:
  sem_t         *m_sem;

};

#endif
