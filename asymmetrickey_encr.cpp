#include "rsa.h"

// ============================================================================
/**
* Encrypt
*/
void encrypt(int argc, char *argv[])
{
  //// c = M^e(mod n)
  //mpz_powm(c,M,e,n);

  //// Get the string representation of the cipher
  //mpz_get_str(ciphertext,10,c);

}

// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  std::cout << "/* =========================" << std::endl;
  std::cout << "/* Asymmetric Key Encryption" << std::endl;
  std::cout << "/* =========================" << std::endl;
  if (argc != 3) {
    std::cerr << "USAGE: ./asymmetrickey_encr <pub_key_file> <file_to_encrypt>" << std::endl;
    return 1;
  }    

  std::string pubKeyFile(argv[1]);
  std::string plainTextFile(argv[2]);

  std::string keyStr = readFirstLineOfFile(pubKeyFile);
  size_t idx = keyStr.find(',');
  std::string eStr(keyStr.substr(0, idx));
  std::string nStr(keyStr.substr(idx + 2));
  
  std::cout << "Read eStr: " << eStr << std::endl;
  std::cout << "Read nStr: " << nStr << std::endl;

  mpz_t e;
  mpz_init(e);
  mpz_set_str(e, eStr.c_str(), 10); // use base 10

  mpz_t n;
  mpz_init(n);
  mpz_set_str(n, nStr.c_str(), 10); // use base 10

  std::cout << "Read e: " << e << std::endl;
  std::cout << "Read n: " << n << std::endl;

  std::string plainTextStr = readFirstLineOfFile(plainTextFile);
  
  
  
 

  // write ciphertext to file

  return 0;
}

