
#include "http_request.hpp"

using namespace std;

HttpRequest::HttpRequest()
  : m_rtype(HTTP_UNSET),
    m_return_count(0)
{
}

HttpRequest::~HttpRequest()
{
}

bool                    HttpRequest::feed(string line)
{
  if (line.size() <= 1)
    {
      m_return_count++;
    }
  else
    {
      if (m_return_count == 1)
        {
          // FIXME ? parse POST/PUT data
        }
      else if (m_return_count == 2)
        {
          process();
        }
      else
        {
          parse_line(line);
        }
      m_return_count = 0;
    }
}

bool                    HttpRequest::parse_line(std::string &line)
{
  string                name, value;
  unsigned int          pos;

  pos = line.find(":");
  name = line.substr(0, pos);
  value = line.substr(pos + 1, line.size());

  if (name ==
}
