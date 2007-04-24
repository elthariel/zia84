

#include <iostream>
#include "mod_psp.hpp"

/*
 * Functions
 */
BufferType      identify_buffer(EZ_IBuffer *a_buf)
{
  if (dynamic_cast<EZ_IBasicRequestBuffer *>(a_buf) != 0)
    return IN;
  else if (dynamic_cast<EZ_IBasicRawBuffer *>(a_buf) != 0)
    return RAW;
  else if (dynamic_cast<EZ_IBasicResponseBuffer *>(a_buf) != 0)
    return OUT;
  else
    return NONE;
}

/*
 * bundle struct
 */
bundle::bundle()
  : request(0), raw_post(0),
    response(0), raw_response(0)
{
}

/*
 * ModPsp class
 */
ModPsp::ModPsp()
  : m_next_mod(0)
{
}

bundle                  ModPsp::make_bundle(int a_req_id)
{
  buflist::iterator     iter = m_bufs.begin();
  buflist::iterator     iter_to_delete;         ;
  bool                  finished = false;
  bool                  post_found = false;
  bundle                b;

  while(!finished && iter != m_bufs.end())
    {
      if ((*iter)->getRequestID() == a_req_id)
        {
          switch (identify_buffer(*iter))
            {
            case IN:
              b.request = *iter;
              break;
            case RAW:
              if (post_found)
                b.raw_response = *iter;
              else
                {
                  b.raw_post = *iter;
                  post_found = true;
                }
              break;
            case OUT:
              b.response = *iter;
              break;
            default:
              break;
            }
          iter_to_delete = iter;
          iter++;
          m_bufs.remove(*iter_to_delete);
          finished = b.request && b.raw_port && b.response && b.raw_response;
        }
    }
  return (b);
}

bool                    ModPsp::have_buffer_bundle(int *a_id)
{
  buflist::iterator     iter = m_bufs.begin();
  bool                  res = false;

  while(!res && iter != m_bufs.end())
    {
      res = have_buffer_bundle_id((*iter)->getRequestID());
      if (res)
        *a_id = (*iter)->getRequestID();

      iter++;
    }
  return res;
}

bool                    ModPsp::have_buffer_bundle_id(int a_req_id)
{
  buflist::iterator     iter = m_bufs.begin();
  bool                  res = false;
  bool                  found[4];
  bool                  raw_flag = false;

  found[IN] = false;
  found[RAW] = false;
  found[OUT] = false;
  found{4] = false;

  while(!res && iter != m_bufs.end())
    {
      if ((*iter)->getRequestID() == a_req_id)
        {
          switch (identify_buffer(*iter))
            {
            case IN:
              found[IN] = true;
              break;
            case RAW:
              if (raw_flag)
                found[4] = true;
              else
                {
                  found[RAW] = true;
                  raw_flag = true;
                }
              break;
            case OUT:
              found[OUT] = true;
              break;
            default:
              break;
            }
        }
      iter++;
      res = found[IN] && found[RAW] && found[OUT] && found[4];
    }
  return res;
}

bool                    ModPsp::needProceed()
{
}

bool                    ModPsp::proceed()
{
}

char                    **ModPsp::make_env(EZ_IBasicRequestBuffer *a_buf)
{
  // FIXME : do me
}


/*
 * Obvious code
 */

const EZ_IModule&       ModPsp::getNext() const
{
  return (m_next_mod);
}

bool                    ModPsp::setNext(EZ_IModule &a_mod)
{
  m_next_mod = &a_mod;
  return (true);
}

EZ_IBuffer              *ModPsp::popBuffer()
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

bool                    ModPsp::pushBuffer(EZ_IBuffer &)
{
  m_bufs.push_back(&a_buf);
  return (true);
}

std::string             ModPsp::getModuleName() const
{
  string                res("mod_psp : ver 0.6.0rc1");

  return (res);
}

std::string             ModPsp::getModuleDescription() const
{
  string                res("mod_psp : allows you to run perl script faster than with the cgi interface");

  return (res);
}

EZ_IModule::ModuleLevel ModPsp::getModuleLevel() const
{
  return EZ_IModule::EZ_PROCEED;
}

void                    ModPsp::setServerConfiguration(const std::string&, const std::string&)
{
}

void                    ModPsp::clean(void)
{
  for (buflist::iterator i = m_bufs.begin();
       i != m_bufs.end(); i++)
    delete *i;
  m_bufs.clear();
}
