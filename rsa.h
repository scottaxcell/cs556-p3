#include <iostream>
#include <string>
#include <ctime>

#include <gmp.h>

#ifndef SGA_RSA_H
#define SGA_RSA_H

struct PublicRSAKey
{
  mpz_t e;
  mpz_t n;
};

struct PrivateRSAKey
{
  mpz_t d;
  mpz_t n;
};

struct RSAKey
{
  PublicRSAKey pubKey;
  PrivateRSAKey privKey;
};

// ============================================================================
/**
* Prints a mpz_t number
*/
void printLargeNumber(mpz_t num)
{
  gmp_printf("%s", num);
}

// ============================================================================
/**
* Generates a RSAKey struct
*/
void findLargePrime(mpz_t largePrime)
{
  // Initialize random number generator
  std::srand(std::time(nullptr)); 
  uint32_t randomNum = rand();
  std::cout << "randomNum: " << randomNum << std::endl;

  mpz_t primeNum;
  // Initialize 0
  mpz_init(primeNum);
  // Set to random number
  mpz_set_ui(primeNum, randomNum);
  // Set to next prime number
  mpz_nextprime(primeNum, primeNum);
  // Set result to prime number found
  mpz_set(largePrime, primeNum);
  
  std::cout << "largePrime: " << largePrime << std::endl;
  //printLargeNumber(largePrime);

  return;
}

// ============================================================================
/**
* Generates a RSAKey struct
*/
RSAKey generareRSAKey()
{
  RSAKey rsaKey;




  return rsaKey;
}

#endif
