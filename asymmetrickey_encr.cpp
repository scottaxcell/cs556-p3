#include <iostream>
#include <string>

#include "rsa.h"



// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  mpz_t p;
  mpz_init(p);
  mpz_t q;
  mpz_init(q);

  findLargePrime(p);
  //printLargeNumber(p);

  return 0;
}

