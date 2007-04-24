

#include <iostream>
#include "mod_psp.hpp"

using namespace std;

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
  : m_next_mod(this)
{
}

bundle                  ModPsp::make_bundle(int a_req_id)
{
  buflist::iterator     iter = m_bufs.begin();
  buflist::iterator     iter_to_delete;
  bool                  delete_pending = false;
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
              b.request = dynamic_cast<EZ_IBasicRequestBuffer *>(*iter);
              if ((dynamic_cast<EZ_IBasicRequestBuffer *>(*iter))->getMethode()
                  != EZ_IBasicRequestBuffer::POST)
                post_found = true;
              break;
            case RAW:
              if (post_found)
                b.raw_response = dynamic_cast<EZ_IBasicRawBuffer *>(*iter);
              else
                {
                  b.raw_post = dynamic_cast<EZ_IBasicRawBuffer *>(*iter);
                  post_found = true;
                }
              break;
            case OUT:
              b.response = dynamic_cast<EZ_IBasicResponseBuffer*>(*iter);
              break;
            default:
              break;
            }
          iter_to_delete = iter;
          delete_pending = true;
          finished = b.request && b.raw_post && b.response && b.raw_response;
        }
      iter++;
      if (delete_pending)
        m_bufs.remove(*iter_to_delete);
      delete_pending = false;
    }
  return (b);
}

bool                    ModPsp::have_buffer_bundle(int *a_id = 0)
{
  buflist::iterator     iter = m_bufs.begin();
  bool                  res = false;

  while(!res && iter != m_bufs.end())
    {
      res = have_buffer_bundle_id((*iter)->getRequestID());
      if (res && a_id)
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
  found[4] = false;

  while(!res && iter != m_bufs.end())
    {
      if ((*iter)->getRequestID() == a_req_id)
        {
          switch (identify_buffer(*iter))
            {
            case IN:
              found[IN] = true;
              if (dynamic_cast<EZ_IBasicRequestBuffer *>(*iter)->getMethode()
                  != EZ_IBasicRequestBuffer::POST)
                found[4] = true;
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
  return have_buffer_bundle();
}

bool                    ModPsp::proceed()
{
  int                   req_id;

  if (have_buffer_bundle(&req_id))
    {
      psp_entry(make_bundle(req_id));
      return true;
    }
  else
    return false;
}

char                    **ModPsp::make_env(EZ_IBasicRequestBuffer *a_buf)
{
  // FIXME : do me
}

void                    ModPsp::psp_entry(bundle a_bundle)
{
  //place session data in the header of the response :
  // in *response, a EZ_IBasicResponseBuffer

  PspData.init_psp(a_bundle.raw_post->getData());
  PspData.replace_all_psp();

  if (PspData.psp_done())
    a_bundle.raw_response->setData(PspData.get_computed_page());
}









/*
 * Obvious code
 */

const EZ_IModule&       ModPsp::getNext() const
{
  return (*m_next_mod);
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

bool                    ModPsp::pushBuffer(EZ_IBuffer &a_buf)
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