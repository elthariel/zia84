//
// socket.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sun Mar 11 17:43:22 2007 Nahlwe
// Last update Mon Apr 23 10:30:38 2007 
//

/*
 * Rien de tout ca nest fixe, le design a ete entame mais
 * nest pas propre du tout seule son existence est garantie
 * sa forme reste indefinie
 */
#ifndef	 __SOCKET_H__
# define __SOCKET_H__
#include <string>
#include <iostream>
#include <pthread.h>
#ifdef XNIX
# include <sys/mman.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef WIN_32
# include <windows.h>
# include <winsock2.h>
#endif
#include <errno.h>
#include <sys/socket.h>

struct FilePath
{
 std::string	Path;
};

class SocketError
{
};

class Socket
{
public:
#ifdef XNIX
  Socket(int fd);
#endif
#ifdef WIN_32
  Socket(SOCKET fd);
#endif
  ~Socket();

  unsigned int	  	SocketDoWriteAll(char *buf, unsigned int len);
  unsigned int	  	SocketDoReadAll(std::string &str);
  unsigned int		SocketWriteAll(void  *buf, unsigned int len);
  unsigned int		SocketReadAll(void *addr, unsigned int size);
  Socket	&operator<<(FilePath &);
  Socket        &operator>>(std::string &);
  Socket        &operator<<(std::string str);
#ifdef XNIX
  int           m_fd;
#endif
#ifdef WIN_32
  SOCKET        m_fd;
#endif
private:
  Socket();

};
#endif
