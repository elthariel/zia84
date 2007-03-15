//
// conf.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:46:20 2007 Nahlwe
// Last update Thu Mar 15 09:45:19 2007 Nahlwe
//

#include <libxml++/libxml++.h>
#include <cstdlib>
#include "conf.hpp"

using namespace std;

namespace __gnu_cxx
{
  size_t        hash<string>::operator()(const string& x) const
  {
    hash<char *> h;
    return      h(x.c_str());
  }
};

using namespace __gnu_cxx;

HttpdConf       *HttpdConf::m_instance = 0;

HttpdConf       &HttpdConf::get()
{
  if (m_instance == 0)
    m_instance = new HttpdConf();
  return *m_instance;
}

HttpdConf       &HttpdConf::init(std::string xml_path)
{
  if (m_instance == 0)
    m_instance = new HttpdConf(xml_path);
  return *m_instance;
}

HttpdConf::HttpdConf()
{
  fill_with_defaults();
  if (validate_xml())
    read_conf();
}

HttpdConf::HttpdConf(std::string a_xml_path)
{
  fill_with_defaults();
  m_xml_path = a_xml_path;
  if (validate_xml())
    read_conf();
}

bool                    HttpdConf::validate_xml()
{
}

void                    HttpdConf::read_conf()
{
}

void                    HttpdConf::fill_with_defaults()
{
}

const std::string       *HttpdConf::get_key(string a_key)
{
  //return (const_cast<const string>(m_conf[a_key]));
  return &(m_conf[a_key]);
}

int                     HttpdConf::get_key_int(string a_key)
{
  return atoi(get_key(a_key)->c_str());
}
