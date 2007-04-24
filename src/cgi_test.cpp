
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef WIN_32
# include <windows.h>
#endif

using namespace std;

int main (int ac, char **av)
{

  for (int i = 0; i < 100000; i++)
    cout << "Hellow world" << endl;

  //  sleep(10);
}
