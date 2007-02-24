//
// conf.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:46:20 2007 Nahlwe
// Last update Sat Feb 24 15:53:39 2007 Nahlwe
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
}

HttpdConf::HttpdConf(std::string a_xml_path)
{
}
