#include "rsa.h"

// ============================================================================
/**
* Decrypt block
*/
void decryptBlock(PrivateRSAKey privKey, mpz_t m, mpz_t c)
{
  // c = m^e mod n
  mpz_powm(c, m, privKey.d, privKey.n);
}

// ============================================================================
/**
* Decrypt plaintext
*/
std::string decryptCipherText(PrivateRSAKey privKey, std::string &cipherText)
{
  std::string plainText;
  return plainText;
}

// ============================================================================
/**
* Write cipher text to file
*/
void writePlainText(std::string& plainText)
{
  std::string fileName("scott-axcell.plaintext");
  std::ofstream ofs(fileName, std::ios::trunc | std::ios::out);
  if (!ofs.good()) {
    std::cerr << "ERROR: not able to write file '" << fileName << "'" << std::endl;
    exit(1);
  }

  ofs << plainText;
  ofs << '\n';
  ofs.close();
}

// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  std::cout << "/* =========================" << std::endl;
  std::cout << "/* Asymmetric Key Decryption" << std::endl;
  std::cout << "/* =========================" << std::endl;
  if (argc != 3) {
    std::cerr << "USAGE: ./asymmetrickey_decr <priv_key_file> <file_to_decrypt>" << std::endl;
    return 1;
  }    

  std::string privKeyFile(argv[1]);
  std::string cipherTextFile(argv[2]);

  std::string keyStr = readFirstLineOfFile(privKeyFile);
  size_t idx = keyStr.find(',');
  std::string dStr(keyStr.substr(0, idx));
  std::string nStr(keyStr.substr(idx + 2));
  
  std::cout << "Read dStr: " << dStr << std::endl;
  std::cout << "Read nStr: " << nStr << std::endl;

  PrivateRSAKey privKey;
  mpz_set_str(privKey.d, dStr.c_str(), 10); // use base 10
  mpz_set_str(privKey.n, nStr.c_str(), 10); // use base 10

  std::cout << "Read privKey.d: " << privKey.d << std::endl;
  std::cout << "Read privKey.n: " << privKey.n << std::endl;

  std::string cipherText = readFirstLineOfFile(cipherTextFile);
  std::string plainText = decryptCipherText(privKey, cipherText);
  std::cout << "CipherText: " << cipherText << std::endl;
  std::cout << "PlainText: " << plainText << std::endl;
  writePlainText(plainText);

  return 0;
}

