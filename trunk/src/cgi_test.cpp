
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

using namespace std;

int main (int ac, char **av)
{

  for (int i = 0; i < 100000; i++)
    WriteFile(GetStdHandle(STD_INPUT_HANDLE), "123456789", 10, 0,  NULL);

  //  sleep(10);
}
