//
// socket.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sun Mar 11 17:43:22 2007 Nahlwe
// Last update Mon Mar 12 02:07:58 2007 Nahlwe
//

/*
 * Rien de tout ca nest fixe, le design a ete entame mais
 * nest pas propre du tout seule son existence est garantie
 * sa forme reste indefinie
 */
class Socket
{
public:
  Socket(int fd);
  ~Socket();

  void          getline(char *&, unsigned int &size);
  void          outline(char *, unsigned int size);

  Socket        &operator>>(string &);

  Socket        &operator<<(string &);
  Socket        &operator<<(unsigned int);
  Socket        &operator<<(int);
private:
  Socket();

  int           m_fd;
};
