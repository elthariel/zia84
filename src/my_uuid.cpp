
#include "my_uuid.hpp"

Uuid    *Uuid::m_instance = 0;

Uuid            &Uuid::get()
{
  if (m_instance == 0)
    m_instance = new Uuid();
  return *m_instance;
}

unsigned int    Uuid::id()
{
  unsigned int  tmp;

  m_mutex.lock();
  tmp = m_count++;
  m_mutex.unlock();
  return tmp;
}

Uuid::Uuid()
  : m_count(0)
{
}
