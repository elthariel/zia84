##
## Makefile.am for zia in /home/elthariel/code/httpd/src/psp
##
## Made by Nahlwe
## Login   <elthariel@lse.epita.fr>
##

noinst_HEADERS =


bin_PROGRAMS = mod_test.so


### Final test modules
mod_test_so_SOURCES = modtest.cpp
mod_test_so_CPPFLAGS = -g -Wall -pthread $(PERL_CCOPTS) \
	-I ../../modapi
mod_test_so_LDADD = -lpthread $(PERL_LDOPTS)
mod_test_so_LDFLAGS = -shared

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
