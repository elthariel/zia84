//
// socket.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sun Mar 11 17:43:30 2007 Nahlwe
// Last update Fri Mar 16 11:33:25 2007 Nahlwe
//
#include <pthread.h>
#include <socket.hpp>

using namespace	std;

Socket::Socket()
{
}

Socket::Socket(int)
{
}

Socket::~Socket()
{
}

void	Socket::SocketWriteAll(int fd, const char *addr, int size)
{
  int	n = 0;

  do
  {
  n = write(m_fd, addr + n, size - n);
  } while(n > 0);

}

void	Socket::SocketWriteAll(int fd, void *addr, int size)
{
  int	n = 0;

  do
  {
  n = write(m_fd, ((char *)addr) + n, size - n);
  } while(n > 0);

}

void	Socket::SocketReadAll(int fd, char *addr, int size)
{
  int	n = 0;

  do
  {
  n = read(m_fd, addr + n, size - n);
  } while(n > 0);

}

void	Socket::SocketReadAll(int fd, std::string &str)
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
	/*XXX 404 */ cout << "Can't	open file" << endl;
  fstat(fd, &st);
  if ((addr = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, fd, 0)) == (void *)-1)
	cout << "Can't	mmap file" << endl;
  SocketWriteAll(m_fd, addr, st.st_size);

  return (*this);
}


Socket        &Socket::operator<<(std::string &str)
{
  SocketWriteAll(m_fd, str.c_str(), str.length());

  return (*this);
}

Socket        &Socket::operator>>(std::string &str)
{
  SocketReadAll(m_fd, str);

  return (*this);
}
