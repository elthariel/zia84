//
// conf.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:46:20 2007 Nahlwe
// Last update Thu Mar  8 19:34:22 2007 Nahlwe
//

#include "conf.hpp"

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
