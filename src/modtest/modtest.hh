/*
** modtest.hh
** Login : <lta@lta-laptop>
** Started on  Sat Apr 21 22:57:51 2007 eltha
** $Id$
**
** Copyright (C) 2007 eltha
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef   	MODTEST_HH_
# define   	MODTEST_HH_

#include <list>
#include <EZ_IModule.h>

class TestMod : public EZ_IModule
{
public:
  typedef std::list<EZ_IBuffer *> buflist;

  TestMod(unsigned int a_id);

  virtual bool                    needProceed();
  virtual bool                    proceed();

  virtual EZ_IModule&             getNext() const;
  virtual bool                    setNext(EZ_IModule &);

  virtual EZ_IBuffer              *popBuffer();
  virtual bool                    pushBuffer(EZ_IBuffer &);

  virtual std::string             getModuleName() const;
  virtual std::string             getModuleDescription() const;
  virtual EZ_IModule::ModuleLevel getModuleLevel() const;

  virtual void                    setServerConfiguration(const std::string&, const std::string&);

  virtual void                    clean(void);

protected:
  TestMod();

  unsigned int          m_id;
  EZ_IModule            *m_next_mod;
  buflist               m_bufs;

};

#endif	    /* !MODTEST_HH_ */
