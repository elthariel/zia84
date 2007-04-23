##
## Makefile.am for zia in /home/elthariel/code/httpd/src/psp
##
## Made by Nahlwe
## Login   <elthariel@lse.epita.fr>
##

noinst_HEADERS =

EXTRA_DIST = zia_psp_api.pl

bin_PROGRAMS = mod_psp.so perlembed


### Final psp modules
mod_psp_so_SOURCES =
mod_psp_so_CPPFLAGS = -g -Wall -pthread $(PERL_CCOPTS)
mod_psp_so_LDADD = -lpthread $(PERL_LDOPTS)
mod_psp_so_LDFLAGS = -shared

perlembed_SOURCES = perlembed.cpp
perlembed_CPPFLAGS = -g -Wall -pthread $(PERL_CCOPTS)
perlembed_LDADD = -lpthread $(PERL_LDOPTS)



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
