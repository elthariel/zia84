
#include <iostream>
#include "perlembed.hpp"

using namespace std;

int main (int ac, char **av)
{
  PerlEmbed     p;
  string        test("print \"tesssssst\";");

  p.init_perl();
  p.eval_perl(test);
}
