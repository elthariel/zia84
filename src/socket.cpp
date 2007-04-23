//
// socket.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sun Mar 11 17:43:30 2007 Nahlwe
// Last update Mon Apr 23 12:55:01 2007 
//
#include <pthread.h>
#include <socket.hpp>

using namespace	std;


Socket::Socket()
{
}
#ifdef XNIX
Socket::Socket(int fd)
#endif
#ifdef WIN_32
Socket::Socket(SOCKET fd)
#endif
{
  m_fd = fd;
}

Socket::~Socket()
{
#ifdef WIN_32
  closesocket(m_fd);
#endif
#ifdef XNIX
  close(m_fd);
#endif
}
unsigned int	Socket::SocketWriteAll(void *buf, unsigned int len)
{
  int r;

#ifdef XNIX
  do r = write(m_fd, (char *)buf, len);
  while ((r == -1 && (errno == EINTR)));
#endif
#ifdef WIN_32
  //  cerr << "Sending : " << (char *)buf << endl;
  r = send(m_fd, (char *)buf, len, 0);
  if (r == SOCKET_ERROR)
    {
      cerr << "Error while sending on a socket : " << WSAGetLastError() << endl;
      r = -1;
    }
#endif
  return (r);
}

unsigned int  Socket::SocketDoWriteAll(char *buf, unsigned int len)
{
  unsigned int written = 0 ;
  while (len)
  {
    int w = SocketWriteAll(buf, len) ;
    if (!w) errno = EPIPE ;
    if (w <= 0) break ;
    written += w ;
    buf += w ;
    len -= w ;
  }

  return (written);
}

unsigned int	Socket::SocketReadAll(void *buf, unsigned int len)
{
  int r;

#ifdef XNIX
  do r = read(m_fd, (char *)buf, len);
  while ((r == -1 && (errno == EINTR)));
#endif
#ifdef WIN_32
  r = recv(m_fd,(char *)buf, len, 0);
  if (r == 0)
    {
      cerr << "Error while receving on a socket : " << WSAGetLastError() << endl;
      r = -1;
    }
#endif
  return (r);
}

unsigned int  Socket::SocketDoReadAll(std::string &str)
{
  unsigned int readden = 0 ;
  unsigned int len = 4096;
  char	buff[4096];
  int w;

  do
  {
    w = SocketReadAll(buff, len) ;
    if (!w) errno = EPIPE ;
    if (w <= 0) break ;
    buff[w] = 0;
    readden += w;
    str += buff;
  }
  while (w == 4096);

  return (readden);
}

Socket        &Socket::operator<<(FilePath &file)
{
  int		fd;
  struct	stat st;
  char		*addr;


  if ((fd = open(file.Path.c_str(), 0400)) == -1)
    {
      cerr << "Can't open file" << endl;
      return (*this);
    }
  fstat(fd, &st);
#ifdef WIN_32
  close(fd);
#endif
#ifdef XNIX
  if ((addr = (char *) mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) == (void *) -1)
  {
    cerr << "Can't mmap file" << endl;
    return (*this);
  }
#endif
#ifdef WIN_32
  HANDLE        map_handle;
  HANDLE        file_handle;

  file_handle = CreateFile(TEXT(file.Path.c_str()),
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
  if (file_handle == INVALID_HANDLE_VALUE)
    {
      cerr << "Unable to open file : " << file.Path.c_str() << " " << GetLastError() << endl;
      return *this;
    }
  else
    {
      map_handle = CreateFileMapping(file_handle,
                                     NULL,
                                     PAGE_READONLY,
                                     0, 0,
                                     NULL);
      if (map_handle == NULL)
        {
          cerr << "Unable to mmap file : " << GetLastError() << endl;
          return *this;
        }
      else
        {
          addr = (char *)MapViewOfFile(map_handle, FILE_MAP_READ,
                                       0, 0, 0);
          if (addr == NULL)
            {
              cerr << "Unable to get a view on the mmap()-ed file : " << GetLastError() << endl;
              return *this;
            }
        }
    }

#endif
  SocketDoWriteAll(addr, st.st_size);
#ifdef XNIX
  munmap(addr, st.st_size);
  close(fd);
#endif
#ifdef WIN_32
  UnmapViewOfFile(addr);
  CloseHandle(map_handle);
  CloseHandle(file_handle);
#endif

  return (*this);
}

Socket        &Socket::operator<<(std::string str)
{
  if(SocketDoWriteAll((char*)str.c_str(), str.length()) !=  str.length())
  {
    throw new SocketError;
  }


  return (*this);
}


Socket        &Socket::operator>>(std::string &str)
{
  if (!SocketDoReadAll(str))
  {
    throw new SocketError;
  }

  return (*this);
}
