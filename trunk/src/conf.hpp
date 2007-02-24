//
// conf.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:41:21 2007 Nahlwe
// Last update Sat Feb 24 15:56:02 2007 Nahlwe
//

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

private:
  HttpdConf();
  HttpdConf(std::string a_xml_path);

  static HttpdConf      *m_instance;

  //Put here configuration data.
};
