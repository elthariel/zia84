//
// http_request.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:55 2007 Nahlwe
// Last update Mon Apr 23 12:51:57 2007 
//

#include <pthread.h>
#include <sys/stat.h>
#include "http_request.hpp"

using namespace std;



const status_s HttpRequest::m_reason[] =
{
{100,"Continue"},
{101,"SwitchingProtocols"},
{200,"OK"},
{201,"Created"},
{202,"Accepted"},
{203,"Non-AuthoritativeInformation"},
{204,"NoContent"},
{205,"ResetContent"},
{206,"PartialContent"},
{300,"MultipleChoices"},
{301,"MovedPermanently"},
{302,"Found"},
{303,"SeeOther"},
{304,"NotModified"},
{305,"UseProxy"},
{307,"TemporaryRedirect"},
{400,"BadRequest"},
{401,"Unauthorized"},
{402,"PaymentRequired"},
{403,"Forbidden"},
{404,"NotFound"},
{405,"MethodNotAllowed"},
{406,"NotAcceptable"},
{407,"ProxyAuthenticationRequired"},
{408,"RequestTimeout"},
{409,"Conflict"},
{410,"Gone"},
{411,"LengthRequired"},
{412,"PreconditionFailed"},
{413,"RequestEntityTooLarge"},
{414,"Request-URITooLong"},
{415,"UnsupportedMediaType"},
{416,"RequestedRangeNotSatisfiable"},
{417,"ExpectationFailed"},
{500,"InternalServerError"},
{501,"NotImplemented"},
{502,"BadGateway"},
{503,"ServiceUnavailable"},
{504,"GatewayTimeout"},
{505,"HTTPVersionNotSupported"},
{0, 0},
};
const char * HttpRequest::m_method [] =  { "OPTIONS" , "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT",  0 };

void HttpRequest::HttpTest()
{
 HttpMap::iterator	i;

  cerr << "chunk type: " << m_chunk_type << endl;
  cerr << "MAP:" << endl;

  for (i = m_http_map.begin(); i != m_http_map.end(); i++)
    {
      cerr << (*i).first << ":" << (*i).second <<  endl;
    }
  cerr << "DATA:" << endl << 	m_data << endl;
}

HttpRequest::HttpRequest(char *filename)
{
  string2 	buff;
  char 	bu[4096];
  int 	n;
  int 	fd  = open(filename, O_RDONLY, 0444);

  n  = read(fd, bu, 4096);
  bu[n] = 0;
  buff += bu;
  m_chunk_type = TYPE_HEADER;
  HttpFill(buff);
}

HttpRequest::HttpRequest(Socket &sock)
{
  string2	buff;
  try
  {
    sock >> buff;

    cout << "Receving" << endl << buff << endl << "---end recv" << endl;
    m_chunk_type = TYPE_HEADER;
    m_http_map["content-length"] = "0";
    m_http_map["version"] = "HTTP/1.1";
    HttpFill(buff);
  }
  catch (SocketError*)
  {
    cout << "read error " << endl; 
  }
}

HttpRequest::~HttpRequest()
{

}

string	HttpRequest::HttpCreateHeader()
{
  string2	header;

  m_http_map["server"] = "zia";
  m_http_map["content-type"] = "text/html"; 	//XXX cahnger le type au bon moment
  header += "server: " + m_http_map["server"] + "\r\n";
  header += "content-type: " + m_http_map["content-type"] + "\r\n";
  header += "content-length: " + m_http_map["content-length"] + "\r\n";
  if (!m_http_map["method"].compare("OPTIONS"))
    header += "Allow: GET, HEAD, OPTIONS, TRACE\r\n" ;
  header += "date: ";
  header.itime();
  header += "\r\n\r\n";


  return (header);
}


string	HttpRequest::HttpGetCGI()
{
  string2	buff;
  string2	buffsize;
#ifdef XNIX
  int		pfd[2];

  pipe(pfd);
  setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
  setenv("REQUEST_METHOD", m_http_map["method"].c_str(), 1);
  setenv("QUERY_STRING", m_http_map["cgi_arg"].c_str(), 1);
  if (!m_http_map["method"].compare("POST"))
    setenv("CONTENT_LENGTH", m_http_map["content-length"].c_str(), 1);

  if (!fork())
  {
    if (!m_http_map["method"].compare("POST"))
      dup2(pfd[0], 0);
    else
      close(pfd[0]);
    dup2(pfd[1], 1);

    execve(m_http_map["cgi"].c_str(), 0, environ);
    close(pfd[1]);
    close(pfd[0]);
    _exit(1);
  }
  else
  {
    if (!m_http_map["method"].compare("POST"))
      write(pfd[1], m_data.c_str(), m_data.length());
    close(pfd[1]);
    wait(0);
    Socket cgi_socket(pfd[0]);
    cgi_socket >> buff;
    close(pfd[0]);
  }
  //XX try catch sur l envoie
  buffsize.itoa(buff.length());
  m_http_map["content-length"] = buffsize;
#endif
  return (buff);
}


int	HttpRequest::HttpCheckRequest(void)
{
  string::size_type	pos;
  string	chunk;
  string2	subchunk;
  string2	chunk2;

  m_status = 400;
  chunk = m_http_map["method"];
  chunk2.append(chunk);
  if (!chunk2.is_in_list(m_method))
    return (1);
  chunk = m_http_map[m_http_map["method"]];
  if ((pos = chunk.rfind("HTTP/")) == string::npos)
    return (1);
  chunk2 = chunk.substr(pos, chunk.length());
  chunk2.strip("\r\n");
  if (!chunk2.split("/", subchunk))
    return (1);
  if (subchunk.find("HTTP") == string::npos)
    return (1);
  if ((pos = chunk2.find("1.1")) == string::npos)
    return (1); //renvoyer si bad version
  if (!HttpCheckHttpMap())
    return (1);
  m_status = 200;
  if (m_http_map["method"].compare("OPTIONS"))
     m_status = HttpSetFile();
 // else
  // m_status = 302;
 // if (!m_http_map["method"].compare("HEAD"))
   //  m_status = 200;
  return (1);
}

int	HttpRequest::HttpCheckHttpMap(void)
{
  if (!(m_http_map["host"].compare("")))
    return (0);
  if (!m_http_map["method"].compare("POST") && !m_http_map["content-length"].compare("0"))
  {
    reqcgi = 0;
    reqfile = 0;
    m_status = 411;
    return (0);
  }
  return (1);
}

std::string	HttpRequest::HttpGetFile(void)
{
   string	path = m_http_map["uri"];
   string	file;
   struct	stat st;
   int		fd;
   int		len;
   int		n; 
   char		buff[4096];

   if ((fd  = open(path.c_str(), O_RDONLY)) == -1)
    {
      cerr << "Can't open file" << endl;
      return ("");
    }
   fstat(fd, &st);
   len = st.st_size;
   do
   {
    n = read(fd, buff, 4096);
    if (n < 1)
      break;
    buff[n] = 0;
    len -= n;
    file += buff;
   }
   while (len);

   return (file);
}

int	HttpRequest::HttpSetFile(void)
{
  std::string chunk;
  string2  chunk2;
  string   filepath;
  struct   stat st;

  reqcgi = 0;
  reqfile = 0;
    
    
  filepath = HttpdConf::get().get_key("/zia/server/root")->c_str();
  chunk = m_http_map[m_http_map["method"]];
  chunk2.append(chunk);
  chunk2.strip("HTTP/1.1");
  chunk2.strip(" ");
  chunk = chunk2;
  
  if ((chunk.find("/") == std::string::npos))
    return (400);
  if (!(chunk.find("*") == std::string::npos))
    return (400);
  if (! (chunk.find("../") == std::string::npos))
    return (403) ;
  if (! (chunk.find("%23") == std::string::npos))
    return (404) ;


  if (!chunk.length())
    return (400);
  if (!chunk.compare("/"))
    filepath += chunk + "index.html";
//  else if (!chunk.(rfind) "/") si le dernier char est un /
//
// set_get_dirlisting
  else
    filepath += chunk;
  if (!filepath.find(HttpdConf::get().get_key("/zia/server/cgi")->c_str()))
  {
    string2 path = "";
    string2 arg;

    arg.append(filepath);
    if (arg.split("?", path))
    {
    m_http_map["cgi"] = path;
    m_http_map["cgi_arg"] = arg;
    filepath = path;
    }
    else
    {
    m_http_map["cgi"] = filepath;
    m_http_map["cgi_arg"] = "";
    }
    reqcgi = 1;
  }

//faire un stat / si c un rep on fait un get dirlist ou on renvoie erreur
//// si pas les drois renvoyer can't stat FORBIDEN
//
  if (stat(filepath.c_str(), &st)  == -1)
  {
    filepath = HttpdConf::get().get_key("/zia/server/root")->c_str();
    filepath += "/error.html";
    stat(filepath.c_str(), &st);
    m_http_map["uri"] = filepath;
    chunk2 = "";
    chunk2.itoa(st.st_size);
    m_http_map["content-length"] = chunk2;
    reqfile = 1;
    reqcgi = 0;
    return (404); //404 met test * 
  }
  else
  {
   if (!reqcgi)
   {
    m_http_map["uri"] = filepath;
    chunk2 = "";
    chunk2.itoa(st.st_size);
    m_http_map["content-length"] = chunk2;
    reqfile = 1;
    }
  }
  return (200); //302
}

int	HttpRequest::HttpParseChunk(string2 &buff)
{
  string2	chunk;

  if (!buff.length())
       return (0);
  if (m_chunk_type != TYPE_DATA)
  {
    if (!buff.split("\r\n", chunk))
  	return (HttpSetChunk(buff));
   return (HttpSetChunk(chunk));
  }
  return (HttpSetChunk(buff));
}


int	HttpRequest::HttpSetHeader(string2 chunk)
{
  string2	header_method;

  chunk.strip("\n");
  chunk.strip("\r");
  chunk.strip("\t");
  chunk.striponce(' ');

  if (!chunk.split(" ", header_method))
	return (0);
  if (!header_method.is_in_list(m_method))
	return (0);
  m_http_map["method"] = header_method;
  m_http_map[header_method] = chunk;
  m_chunk_type = TYPE_MAP;
  return (1);
}

int	HttpRequest::HttpSetMap(string2 chunk)
{
  string2	key;

  if (!chunk.length())
  {
    m_chunk_type = TYPE_DATA;
    return (1);
  }
  if (!chunk.split(":", key))
     return (0);
  key.tolower();
  key.strip(" ");
  key.strip("	");
  m_http_map[key] = chunk;

  return (1);
}

int	HttpRequest::HttpSetData(string2 &chunk)
{
 m_data = chunk;
 chunk = "";

 return (1);
}

int	HttpRequest::HttpSetChunk(string2 &chunk)
{
  if (m_chunk_type == TYPE_DATA)
    return (HttpSetData(chunk));
  if (m_chunk_type == TYPE_HEADER)
    return (HttpSetHeader(chunk));
  if (m_chunk_type == TYPE_MAP)
   return (HttpSetMap(chunk));
  return (0);
}

void	HttpRequest::HttpFill(string2 buff)
{
  while (HttpParseChunk(buff));
}


std::string HttpRequest::HttpGetReason(void)
{
  int i;

  for (i = 0; m_reason[i].code != m_status ; i++);
  if (m_reason[i].reason)
    return (m_reason[i].reason);
  return ("");
}

std::string HttpRequest::HttpGetStatus(void)
{
  string2  code;
  string  status;

  status = m_http_map["version"];
  code.itoa(m_status);
  status += " ";
  status += code;
  status += " ";
  status += HttpGetReason();
  status += "\r\n";
  return (status);
}

