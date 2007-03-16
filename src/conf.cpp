//
// conf.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:46:20 2007 Nahlwe
// Last update Fri Mar 16 03:05:12 2007 Nahlwe
//

#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include "conf.hpp"

using namespace std;
using namespace xmlpp;

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

HttpdConf       &HttpdConf::init(const std::string xml_path)
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
  bool                  ret = true;

  m_parser.set_validate(true);
  try
    {
      m_parser.parse_file(m_xml_path);
      ret = m_parser;
    }
  catch (...)
    {
      cerr << "We were unable to validate file (Existence or DTD) : "
           << m_xml_path << endl;
      ret = false;
    }
  return ret;
}

void                    HttpdConf::read_conf()
{
  const Node            *nd;

  cerr << "Conf : Started reading configuration" << endl;
  nd = m_parser.get_document()->get_root_node();
  _rec_read_conf(nd, "");
  dump_config();
}

void                    HttpdConf::_rec_read_conf(const xmlpp::Node *a_node,
                                                  string a_node_path)
{
  string                nodename = a_node->get_name();

  if (nodename.empty())
    return;

  const TextNode *nodeText = dynamic_cast<const TextNode*>(a_node);
  const Element *nodeElement = dynamic_cast<const Element*>(a_node);


  if (nodeText)
    {
      if (nodeText->is_white_space())
        return;
      m_conf[a_node_path] = nodeText->get_content();
    }
  else if (nodeElement)
    {
      Node::NodeList            children = nodeElement->get_children();
      Node::NodeList::iterator  i = children.begin();

      a_node_path += string("/");
      a_node_path += nodename;

      for (; i != children.end(); i++)
        _rec_read_conf(*i, a_node_path);
    }
  else
    {
      cerr << "Xml zia conf parsing error : "
           << a_node_path << endl;
    }
}

void                    HttpdConf::dump_config()
{
  cerr << "Conf : Dumping config :" << endl;
  ConfMap::iterator     i;
  for (i = m_conf.begin(); i != m_conf.end(); i++)
    {
      cerr << (*i).first << " : "
           << (*i).second << endl;
    }
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
