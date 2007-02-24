//
// http_request.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:55 2007 Nahlwe
// Last update Sat Feb 24 15:32:00 2007 Nahlwe
//

#include "http_request.hpp"

using namespace std;

/*
 * Uri class
 */

//a_uri.npos

Uri::Uri(std::string a_uri)
{
  if (a_uri.find("http://", 0, 8) != a_uri.npos) //FIXME check https
    {
      m_type = UriAbs_Host;
      // FIXME parse
    }
  else if (a_uri.c_str()[0] == /)
    {
      m_type = UriAbs;
      // FIXME parse
    }
  else // FIXME check for ../[../]etc/passwd-like uri :p
    {
      m_type = UriRel;

    }

}

Uri::Uri()
{
}

const std::string       &Uri::proto()
{
  return m_proto;
}

const std::string       &Uri::host()
{
  return m_host;
}

const std::string       &Uri::path()
{
  return m_path;
}

const UriType           &Uri::type()
{
  return m_type;
}


/*
 * HttpRequest class
 */

HttpRequest::HttpRequest()
  : m_rtype(HTTP_UNSET),
    m_return_count(0)
{
}

HttpRequest::~HttpRequest()
{
}

bool                    HttpRequest::feed(string line)
{
  if (line.size() <= 1)
    {
      m_return_count++;
    }
  else
    {
      if (m_return_count == 1)
        {
          // FIXME ? parse POST/PUT data
        }
      else if (m_return_count == 2)
        {
          process();
        }
      else
        {
          parse_line(line);
        }
      m_return_count = 0;
    }
}

bool                    HttpRequest::parse_line(std::string &line)
{
  string                name, value;
  unsigned int          pos;

  pos = line.find(":");
  name = line.substr(0, pos);
  value = line.substr(pos + 1, line.size());

  if (name ==
}
