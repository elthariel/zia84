//
// conf.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:41:21 2007 Nahlwe
// Last update Thu Mar 15 09:44:47 2007 Nahlwe
//

#include <string>
#include <ext/hash_map>

namespace __gnu_cxx
{
  template<>
  struct hash<std::string>
  {
  public:
    size_t operator()(const std::string& x) const;
  };
}

/*
 * This is just a singleton used to retrieve configuration
 * of the server from anywhere. And to make change takes effects
 * immediately from anywhere a change is made.
 */
class HttpdConf
{
public:
  static HttpdConf      &get();
  static HttpdConf      &init(std::string xml_path);

  //Put here accessors for anything you want
  // Acces format of the config options will be chosen by
  // the one who will impleemnt this features.
  const std::string     *get_key(std::string a_key);
  int                   get_key_int(std::string a_key);

private:
  HttpdConf();
  HttpdConf(std::string a_xml_path);

  /**
   * Return true if the xml document exists and is valid regarding the
   * DTD.
   */
  bool                  validate_xml();

  /**
   * Read the configuration from the xml file.
   */
  void                  read_conf();

  /**
   * Fill all the params with default values.
   * Including the path for the xml config file.
   */
  void                  fill_with_defaults();

  static HttpdConf      *m_instance;

  //Put here configuration data.
  std::string           m_xml_path;
  __gnu_cxx::hash_map<std::string,
                      std::string,
                      __gnu_cxx::hash<std::string> > m_conf;
};
