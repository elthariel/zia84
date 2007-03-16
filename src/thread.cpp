//
// thread.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 08:05:03 2007 Nahlwe
// Last update Fri Mar 16 07:24:20 2007 Nahlwe
//

#include <iostream>
#include <sched.h>
#include "thread.hpp"

using namespace std;

/*
 * Thread class
 */
void            *run_thread(void *arg)
{
  iFoncteur0<void>      *fonc = (iFoncteur0<void>*)arg;
  (*fonc)();
  return 0;
}

Thread::Thread(iFoncteur0<void> &a_foncteur)
  : m_fonc(a_foncteur)
{
  if (pthread_create(&m_thread, 0,
                     &run_thread, (void *)&a_foncteur) != 0)
    cerr << "Thread wasn't created" << endl;
}

Thread::~Thread()
{
  pthread_cancel(m_thread);
}

void            Thread::thread_func()
{
}

void            Thread::join()
{
  pthread_join(m_thread, 0);
}

bool            Thread::operator==(Thread &a_th)
{
  if (pthread_equal(m_thread, a_th.m_thread) == 0)
    return false;
  else
    return true;
}

bool            Thread::operator!=(Thread &a_th)
{
  if (pthread_equal(m_thread, a_th.m_thread) != 0)
    return false;
  else
    return true;
}

void            Thread::sleep(unsigned int a_usecs)
{
  usleep(a_usecs);
}

void            Thread::yield()
{
  sched_yield();
}

/*
 * Mutex class
 */

Mutex::Mutex()
{
  pthread_mutex_init(&m_mutex, 0);
}

Mutex::~Mutex()
{
  pthread_mutex_destroy(&m_mutex);
}

void                    Mutex::lock()
{
  pthread_mutex_lock(&m_mutex);
}

void                    Mutex::unlock()
{
  pthread_mutex_unlock(&m_mutex);
}

int                     Mutex::trylock()
{
  return   pthread_mutex_trylock(&m_mutex);
}

/*
 * Event class
 */

Event::Event()
{
  pthread_cond_init(&m_event, 0);
}

Event::~Event()
{
  pthread_cond_destroy(&m_event);
}

int                     Event::wake_all()
{
  return pthread_cond_broadcast(&m_event);
}

int                     Event::wake_one()
{
  return pthread_cond_signal(&m_event);
}

void                    Event::wait()
{
  lock();
  pthread_cond_wait(&m_event, &m_mutex);
  unlock();
}

void                    Event::wait(unsigned int a_sec)
{
  timespec              time;

  time.tv_nsec = 0;
  time.tv_sec = a_sec;

  lock();
  pthread_cond_timedwait(&m_event, &m_mutex, &time);
  unlock();
}
