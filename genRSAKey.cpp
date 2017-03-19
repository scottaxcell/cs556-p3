#include <iostream>
#include <string>
#include <ctime>

#include "rsa.h"

// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  std::srand(std::time(nullptr)); 

  RSAKey rsaKey = generareRSAKey();

  return 0;
}

