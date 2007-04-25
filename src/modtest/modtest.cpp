#include <iostream>
#include "modtest.hh"
#include <EZ_IStreamBuffer.h>

using namespace std;

TestMod::TestMod(unsigned int a_id)
  : m_id(a_id),
    m_next_mod(this)
{
  cerr << "Create TestMod #" << a_id << endl;
}

bool                    TestMod::needProceed()
{
  return (true);
}

bool                    TestMod::proceed()
{
  // FIXME
  cerr << m_id << " : ModTest::proceed()" << endl;
  while (m_bufs.begin() != m_bufs.end())
    {
      EZ_IBuffer *b = popBuffer();
      m_bufs.pop_front();
      if (b)
        getNext().pushBuffer(*b);
    }
  return (true);
}

EZ_IModule&             TestMod::getNext() const
{
  return (*m_next_mod);
}

bool                    TestMod::setNext(EZ_IModule &a_mod)
{
  m_next_mod = &a_mod;
  return (true);
}

EZ_IBuffer              *TestMod::popBuffer()
{
  EZ_IBuffer            *buf;

  if (m_bufs.size() > 0)
    {
      buf = *m_bufs.begin();
      m_bufs.pop_front();
      return buf;
    }
  else
    return 0;
}

bool                    TestMod::pushBuffer(EZ_IBuffer &a_buf)
{
  cerr << m_id << " : You pushed a buffer" << endl;
  m_bufs.push_back(&a_buf);
  return (true);
}

std::string             TestMod::getModuleName() const
{
  return ("Zia TestMod");
}

std::string             TestMod::getModuleDescription() const
{
  return ("Zia TestMod : outputs infos on the buffer which pass through itself");
}

EZ_IModule::ModuleLevel TestMod::getModuleLevel() const
{
  return (EZ_IModule::EZ_IN);
}

void                    TestMod::setServerConfiguration(const std::string&, const std::string&)
{
}

void                    TestMod::clean(void)
{
  for (buflist::iterator i = m_bufs.begin();
       i != m_bufs.end(); i++)
    delete *i;
  m_bufs.clear();
}

TestMod::TestMod()
{
}

/*
 * Module c api
 */
EXPORT EZ_IModule	*getInstance()
{
  static unsigned int   instance_id= 0;

  return (new TestMod(instance_id++));
}
