##
## Makefile.am for zia in /home/elthariel/code/httpd/src/psp
##
## Made by Nahlwe
## Login   <elthariel@lse.epita.fr>
##

noinst_HEADERS = mod_psp.hpp bloc.hpp perlembed.hpp

EXTRA_DIST = zia_psp_api.pl

bin_PROGRAMS = mod_psp.so #perlembed


### Final psp modules
mod_psp_so_SOURCES = mod_psp.cpp bloc.cpp perlembed.cpp \
	psp_main.cpp thread.cpp foncteurs.cpp
mod_psp_so_CPPFLAGS = -fPIC -g -Wall -pthread $(PERL_CCOPTS) \
	-I../../modapi
mod_psp_so_LDADD = -lpthread $(PERL_LDOPTS) -lrt
mod_psp_so_LDFLAGS = -shared -fPIC

# perlembed_SOURCES = perlembed.cpp bloc.cpp test.cpp thread.cpp \
# 	foncteurs.cpp
# perlembed_CPPFLAGS = -g -Wall -pthread $(PERL_CCOPTS)
# perlembed_LDADD =  -lpthread $(PERL_LDOPTS) -lrt



##########
# Checks #
##########

# --> make check
#TESTS = test-1.sh test-
#TESTS = zia_test.sh
#check_PROGRAMS = test-2
#test_2_SOURCES = main_for_tests.cc


####################
# Additional Rules #
####################

re : clean all

c : clean

a : clean
