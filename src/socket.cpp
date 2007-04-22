//
// socket.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sun Mar 11 17:43:30 2007 Nahlwe
// Last update Sat Mar 17 01:58:01 2007 Nahlwe
//
#include <pthread.h>
#include <socket.hpp>

using namespace	std;

Socket::Socket()
{
}

Socket::Socket(int fd)
{
  m_fd = fd;
}

Socket::~Socket()
{
  close(m_fd);
}

void	Socket::SocketWriteAll(const char *addr, int size)
{
  int	n = 0;
  int	nsz = 0;

  do
  {
    n = write(m_fd, addr + nsz, size - nsz); 
    nsz += n;
  }
  while(n > 0 && (size - nsz) > 0);
}

void	Socket::SocketWriteAll(void *addr, int size)
{
  int	n = 0;
  int	nsz = 0;

  do
  {
    n = write(m_fd, ((char *)addr) + nsz, size - nsz);
    nsz += n;
  }
  while (n > 0 && (size - nsz) > 0);

}

void	Socket::SocketReadAll(char *addr, int size)
{
  int	n = 0;
  int	nsz = 0;

  do
  {
    n += read(m_fd, addr + n, size - n);
    nsz += n;
  }
  while (n > 0 && (size - nsz) > 0);
}

void	Socket::SocketReadAll(std::string &str)
{
  int	n;
  char	buff[1024];

  do
  {
  n = read(m_fd, buff, 1024);
  buff[n] = 0;
  str += buff;
  } while (n == 1024);

}

Socket        &Socket::operator<<(FilePath &file)
{
  int		fd;
  struct	stat st;
  void		*addr;

  if ((fd = open(file.Path.c_str(), 0400)) == -1)
  {
    cerr << "Can't open file" << endl;
    return (*this);
  }
  fstat(fd, &st);
  if ((addr = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) == (void *) -1)
  {
    cerr << "Can't mmap file" << endl;
    return (*this);
  }
  SocketWriteAll(addr, st.st_size);
  munmap(addr, st.st_size);
  close(fd);

  return (*this);
}

/*
Socket        &Socket::operator<<(std::string &str)
{
  SocketWriteAll(str.c_str(), str.length());

  return (*this);
}
*/
Socket        &Socket::operator<<(std::string str)
{
  SocketWriteAll(str.c_str(), str.length());

  return (*this);
}


Socket        &Socket::operator>>(std::string &str)
{
  SocketReadAll(str);

  return (*this);
}
