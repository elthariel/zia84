//
// design_ideas.hpp for zia in /home/elthariel/code/httpd
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 16 06:50:19 2007 Nahlwe
// Last update Fri Feb 23 10:49:30 2007 Nahlwe
//

/* |---------------|
 * |/!\ Warning /!\|
 * |---------------|
 *
 * This document just contains ideas and is subject to deep
 * modificaton depending on group's opinions.
 *
 */

/*
  Modele du serveur :

  L'idee, pour le moment, est de partir sur la base d'un serveur
  a base de workers, qui se presentent sous la forme de threads.

  Globalement cela se manifeste  sous la forme d'un thread
  principal qui va bloquer sur le socket principal et accepter
  les connections entrantes, ainsi que d'un pool de thread
  qui se verront attribuer les connections à traiter.

  Cependant, a cause des problemes lies a la portabilite de notre
  serveur sous windows, il nous faut trouver un moyen de changer
  la configuration pendant l'execution. Sous Unix, un domain
  socket gere par le thread principal aurait ete une solution
  acceptable, cependant cette solution n'est pas viable sous
  WinXX. On peut donc envisager d'utilier un signal pour signaler
  un chgt de configuration, ou bien un autre port. --> A discuter.
 */


/*

Once a request have been dispatched to a worker the following steps
are taken to send the document back to the client :

      (hook point) -> ?
  - Parse beginning of the request to determine requested document.
      (hook point) -> SSL, VHost
  - Parse end of the the request : determine POST & GET & etc
      (hook point) -> Cookie security
  - Read file from disk / Exec CGI
      (hook point) -> Embedded interpreter (Perl, PHP)
  - Send file back to the client
      (hook point) -> Statistic

*/

#include <vector>

template <class return_type>
class foncteur0
{
public:
  virtual return_type   operator()() = 0;
};

class Thread
{
public:
};

class Mutex
{
};

class Event
{
};

class Worker : public Thread
{
public:
};

class WorkerPool
{
public:
private:
  vector<Worker &>      m_pool;
};
