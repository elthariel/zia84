

#include <iostream>
#include "mod_psp.hpp"

using namespace std;

/*
 * Functions
 */
EZ_IBuffer::TYPE      identify_buffer(EZ_IBuffer *a_buf)
{
  /*  if (dynamic_cast<EZ_IBasicRequestBuffer *>(a_buf) != 0)
    return IN;
  else if (dynamic_cast<EZ_IBasicRawBuffer *>(a_buf) != 0)
    return RAW;
  else if (dynamic_cast<EZ_IBasicResponseBuffer *>(a_buf) != 0)
    return OUT;
  else
  return NONE;*/
  return (a_buf->getBufferType());
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
            case EZ_IBuffer::EZ_REQUEST:
              b.request = static_cast<EZ_IBasicRequestBuffer *>(*iter);
              /*              if ((static_cast<EZ_IBasicRequestBuffer *>(*iter))->getMethode()
                  != EZ_IBasicRequestBuffer::POST)
                  post_found = true;*/
              break;
            case EZ_IBuffer::EZ_RAW:
              if (post_found)
                b.raw_response = static_cast<EZ_IBasicRawBuffer *>(*iter);
              else
                {
                  b.raw_post = static_cast<EZ_IBasicRawBuffer *>(*iter);
                  post_found = true;
                }
              break;
            case EZ_IBuffer::EZ_RESPONSE:
              b.response = static_cast<EZ_IBasicResponseBuffer*>(*iter);
              break;
            default:
              cerr << "I'm not a buffer ?" << endl;
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

  cout << "Checking bundle for " << a_req_id << endl;

  while(!res && iter != m_bufs.end())
    {
      if ((*iter)->getRequestID() == a_req_id)
        {
          switch (identify_buffer(*iter))
            {
            case EZ_IBuffer::EZ_REQUEST:
              cout << "found in" << endl;
              found[IN] = true;
              if (static_cast<EZ_IBasicRequestBuffer *>(*iter)->getMethode()
                  != EZ_IBasicRequestBuffer::POST)
                found[4] = true;
              break;
            case EZ_IBuffer::EZ_RAW:
              cout << "found raw" << endl;
              if (raw_flag)
                found[4] = true;
              else
                {
                  found[RAW] = true;
                  raw_flag = true;
                }
              break;
            case EZ_IBuffer::EZ_RESPONSE:
              cout << "found out" << endl;
              found[OUT] = true;
              break;
            default:
              cerr << "I'm not a buffer ?" << endl;
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
  bundle                b;

  cout << "Psp proceed()" << endl;
  if (have_buffer_bundle(&req_id))
    {
      cout << "psp_entry" << endl;
      b = make_bundle(req_id);
      psp_entry(b);
      return true;
    }
  else
    return false;
}

char			**ModPsp::make_env(EZ_IBasicRequestBuffer *a_buf)
{
  MHEADERS::const_iterator	p;
  char			**env;
  unsigned int		i;

  if (a_buf == 0)
    return 0;
  env = (char **)malloc(sizeof(char *) * a_buf->getHeaders().size() + 1);
  p = a_buf->getHeaders().begin();
  i = 0;
  while (p != a_buf->getHeaders().end() && i < a_buf->getHeaders().size())
    {
      *(env + i) = (char *)malloc(strlen(p->first.c_str()) + strlen((p->second.c_str()) + 2) * sizeof(char));
      if (*(env + i) == NULL)
	{
	  cerr << "Unable to alloc more env" << endl;
	  return (0);
	}
      if ((strncpy(*(env + i),
		   p->first.c_str(),
		   strlen(p->first.c_str()) - 1)) == NULL)
	{
	  cerr << "Unable to copy env key" << endl;
	  return (0);
	}
      *(*(env + i) + strlen(p->first.c_str())) = '=';
      if ((strncpy(*(env + i + strlen(p->first.c_str())),
		   p->second.c_str(),
		   strlen(p->second.c_str()) - 1)) == NULL)
	{
	  cerr << "Unable to copy env value" << endl;
	  return (0);
	}
      *(*(env + i) + strlen(p->first.c_str())) = 0;
      i++;
    }
  *(env + i) = 0;
  //XX free env ...
  return(env);
}

void                    ModPsp::psp_entry(bundle a_bundle)
{
  // Psp::env in Psp object must have been filled by ModPsp::make_env()
  PspData.init_psp(a_bundle.raw_response->getData(),
		   make_env(a_bundle.request));
  PspData.replace_all_psp();

  if (PspData.psp_done())
    a_bundle.raw_response->setData(PspData.get_computed_page());
}









/*
 * Obvious code
 */

EZ_IModule&		ModPsp::getNext() const
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

EXPORT EZ_IModule	*getInstance()
{
  //  static unsigned int	instance

  return (new ModPsp());
}
