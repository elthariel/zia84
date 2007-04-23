
#include "thread.hpp"

/*
 * Use Uuid::get().id() to get an unique RequestID for
 * EvilZia modules.
 * (max_uint32_t > max_open_fd)
 */
class Uuid
{
public:
  static Uuid   &get();
  unsigned int  id();
protected:
  Uuid();
  static Uuid   *m_instance;
  unsigned int  m_count;
  Mutex         m_mutex;
};
