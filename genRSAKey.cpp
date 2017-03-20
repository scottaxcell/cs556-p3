#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "rsa.h"

// ============================================================================
/**
* Write public key file
*/
void writePublicKey(PublicRSAKey pubKey)
{
  std::string fileName("pub.pem");
  std::ofstream ofs(fileName, std::ios::trunc | std::ios::out);
  if (!ofs.good()) {
    std::cerr << "ERROR: not able to write file '" << fileName << "'" << std::endl;
    exit(1);
  }

  std::ostringstream oss;
  oss << pubKey.e;
  oss << ", ";
  oss << pubKey.n;
  oss << "\n";
  ofs << oss.str();
  ofs.close();
}

// ============================================================================
/**
* Write private key file
*/
void writePrivateKey(PrivateRSAKey privKey)
{
  std::string fileName("priv.pem");
  std::ofstream ofs(fileName, std::ios::trunc | std::ios::out);
  if (!ofs.good()) {
    std::cerr << "ERROR: not able to write file '" << fileName << "'" << std::endl;
    exit(1);
  }

  std::ostringstream oss;
  oss << privKey.d;
  oss << ", ";
  oss << privKey.n;
  oss << "\n";
  ofs << oss.str();
  ofs.close();
}

// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  std::srand(std::time(nullptr)); 

  RSAKey rsaKey = generareRSAKey();

  writePublicKey(rsaKey.pubKey);

  writePrivateKey(rsaKey.privKey);
  
  return 0;
}

