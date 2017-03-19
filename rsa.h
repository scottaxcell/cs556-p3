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
* 
*/

// ============================================================================
/**
* Generates a RSAKey struct
*/
void findLargePrime(mpz_t largePrime)
{
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
  mpz_t _e;
  mpz_init(_e);
  
  mpz_t decr;
  mpz_init(decr);
  mpz_set_ui(decr, 1);
  mpz_sub(_e, totient, decr);

  while (!isEValid(totient, _e))
    mpz_sub(_e, _e, decr);
  
  mpz_set(e, _e);
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

  // TODO e and d are the same number, look into this!

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
