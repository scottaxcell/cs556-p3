#ifndef SGA_RSA_H
#define SGA_RSA_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

#include <gmp.h>
#include <gmpxx.h>

#define KEYSIZE 1024//4096
#define PRIMESIZE (KEYSIZE / 2)

struct PublicRSAKey
{
  mpz_t e;
  mpz_t n;
  PublicRSAKey()
  {
    mpz_init(e);
    mpz_init(n);
  };
};

struct PrivateRSAKey
{
  mpz_t d;
  mpz_t n;
  PrivateRSAKey()
  {
    mpz_init(d);
    mpz_init(n);
  };
};

struct RSAKey
{
  PublicRSAKey pubKey;
  PrivateRSAKey privKey;
};

// ============================================================================
/**
* Reads first line of a file
*/
std::string readFirstLineOfFile(std::string &fileName)
{
  std::ifstream ifs(fileName, std::ios::in);
  if (!ifs.good()) {
    std::cerr << "ERROR: not able to open file '" << fileName << "'" << std::endl;
    exit(1);
  }

  std::string line;
  std::getline(ifs, line);
  ifs.close();

  std::cout << "Read: " << line << std::endl;

  return line;
}

// ============================================================================
/**
* Find a large prime number
*/
void findLargePrime(mpz_t largePrime)
{
  char *str = new char[PRIMESIZE + 1];
  str[0] = '1';
  for (int i = 1; i < PRIMESIZE; i++) {
    int binary = (int)(rand() % 2);
    str[i] = binary + '0';
  }
  str[PRIMESIZE] = '\0';

  mpz_t primeNum;
  mpz_init(primeNum);
  mpz_set_str(primeNum, str, 2);
  mpz_nextprime(primeNum, primeNum);
  mpz_set(largePrime, primeNum);

  free(str);
  //std::cout << "largePrime: " << largePrime << std::endl;
}

// ============================================================================
/**
* Finds the totient of the two numbers provided
*/
void calculateTotient(mpz_t totient, mpz_t p, mpz_t q)
{
  mpz_t _totient, _p, _q;
  mpz_init(_totient);
  mpz_init(_p);
  mpz_init(_q);

  mpz_set(_p, p);
  mpz_sub_ui(_p, _p, 1);
  std::cout << "(p - 1): " << _p << std::endl;
  mpz_set(_q, q);
  mpz_sub_ui(_q, _q, 1);
  std::cout << "(q - 1): " << _q << std::endl;

  mpz_mul(_totient, _p, _q);
  mpz_set(totient, _totient);
}

// ============================================================================
/**
* Verify e is valid
*/
bool isEValid(mpz_t totient, mpz_t e)
{
  mpz_t gcd;
  mpz_init(gcd);

  mpz_gcd(gcd, e, totient);

  // Greatest common denominator is 1
  if (mpz_cmp_ui(gcd, 1) == 0)
    return true;

  return false;
}

// ============================================================================
/**
* Find e, number between 1 and the totient, such that no prime factor is shared
* with the totient
*/
void findE(mpz_t totient, mpz_t e)
{
  // Suggested value of e is, prime number 65537, to make calculations faster 
  unsigned long int _e = 65537;
  mpz_t gcd;
  mpz_init(gcd);
  
  while (true) {
    mpz_gcd_ui(gcd, totient, _e);
    if (mpz_cmp_ui(gcd, (unsigned long int)1) == 0)
      break;
    _e += 2;
  }
  
  mpz_set_ui(e, _e);
}

// ============================================================================
/**
* Find d, where e * d = 1 mod totient
*/
void findD(mpz_t e, mpz_t totient, mpz_t d)
{
  mpz_t _d;
  mpz_init(_d);
  
  // int mpz_invert (mpz_t rop, const mpz_t op1, const mpz_t op2)
  // Compute the inverse of op1 modulo op2 and put the result in rop.
  if (!mpz_invert(_d, e, totient))
    std::cerr << "Could not find mpz_inverse" << std::endl;

  mpz_set(d, _d);
}

// ============================================================================
/**
* Generates a RSAKey struct
*/
RSAKey generareRSAKey()
{
  RSAKey rsaKey;

  // Find two large prime numbers, p and q
  mpz_t p;
  mpz_init(p);
  findLargePrime(p);

  mpz_t q;
  mpz_init(q);
  findLargePrime(q);

  // Calculate n, p * q
  mpz_t n;
  mpz_init(n);
  mpz_mul(n, p, q);

  std::cout << "p: " << p << std::endl;
  std::cout << "q: " << q << std::endl;
  std::cout << "n: " << n << std::endl;

  // Calcualte totient of p and q
  mpz_t totient;
  mpz_init(totient);
  calculateTotient(totient, p, q);
  std::cout << "totient: " << totient << std::endl;

  // Find e
  mpz_t e;
  mpz_init(e);
  findE(totient, e);
  std::cout << "e: " << e << std::endl;

  // Find d
  mpz_t d;
  mpz_init(d);
  findD(e, totient, d);
  std::cout << "d: " << d << std::endl;

  PublicRSAKey pubKey;
  mpz_set(pubKey.e, e);
  mpz_set(pubKey.n, n);

  PrivateRSAKey privKey;
  mpz_set(privKey.d, d);
  mpz_set(privKey.n, n);

  rsaKey.pubKey = pubKey;
  rsaKey.privKey = privKey;

  return rsaKey;
}

#endif
