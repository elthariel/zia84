##
## Makefile.am for zia in /home/elthariel/code/httpd
##
## Made by Nahlwe
## Login   <elthariel@lse.epita.fr>
##
## Started on  Fri Feb 16 04:43:47 2007 Nahlwe
## Last update Fri Mar 16 07:38:23 2007 Nahlwe
##

noinst_HEADERS = foncteurs.hpp help.hpp \
	thread.hpp worker.hpp main.hpp conf.hpp http_request.hpp socket.hpp dired.hpp

#######################
# Build an executable #
#######################

#INCLUDES = -I $(top_builddir)/src

bin_PROGRAMS = zia
zia_SOURCES = foncteurs.cpp help.cpp thread.cpp \
	worker.cpp main.cpp conf.cpp http_request.cpp socket.cpp dired.cpp

# Don't forget that the order of the libraries _does_ matter
zia_CPPFLAGS = -g -Wall -pthread $(LIBXML_CFLAGS)
zia_LDADD = -lpthread $(LIBXML_LIBS) #dir1/libdir1.a

##########
# Checks #
##########

# --> make check
#TESTS = test-1.sh test-2
#check_PROGRAMS = test-2
#test_2_SOURCES = main_for_tests.cc


####################
# Additional Rules #
####################

re : clean all

c : clean

a : clean
