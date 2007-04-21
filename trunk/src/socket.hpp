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
#ifndef	 __SOCKET_H__
#define __SOCKET_H__
#include <string>
#include <iostream>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct FilePath
{
 std::string	Path;
};


class Socket
{
public:
  Socket(int fd);
  ~Socket();

  void		SocketWriteAll(int fd, const char *addr, int size);
  void		SocketWriteAll(int fd, void *addr, int size);
  void		SocketReadAll(int fd, char *addr, int size);
  void		SocketReadAll(int fd, std::string &str);
  Socket	&operator<<(FilePath &);	
  Socket        &operator>>(std::string &);
  Socket        &operator<<(std::string &);
  Socket        &operator<<(std::string);
  int           m_fd;
private:
  Socket();

};
#endif
