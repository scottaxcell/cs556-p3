#include "rsa.h"

// ============================================================================
/**
* Encrypt block
*/
void encryptBlock(PublicRSAKey pubKey, mpz_t m, mpz_t c)
{
  // c = m^e mod n
  mpz_powm(c, m, pubKey.e, pubKey.n);
}

// ============================================================================
/**
* Encrypt plaintext
*/
std::string encryptPlainText(PublicRSAKey pubKey, std::string &plainText)
{
  std::string cipherText;

  int i = 0;
  int blockSize = 100;
  std::string blockStr;
  std::string::const_iterator it = plainText.begin();
  std::string::const_iterator end = plainText.end();
  for ( ; it != end; ++it) {
    if ((i == (blockSize - 1)) ||
        (std::distance(it, end) == 1)) {
      // Have a block of correct size or reached the end of the string
      blockStr.push_back(*it);
      std::cout << "block: '" << blockStr << "' size: " << blockStr.size() << std::endl;

      // Convert m to ascii integer equivalent
      char *intStr = new char[blockStr.size() * 3 + 1];
      intStr[0] = '\0';
      for (int j = 0; j < blockStr.size(); j++) {
        int tmpInt = (int)blockStr[j];
        char tmpChars[4];
        sprintf(tmpChars, "%03d", tmpInt);
        strcat(intStr, tmpChars);
      }
      std::cout << "intStr: '" << intStr << "' size: " << strlen(intStr) << std::endl;

      mpz_t m;
      mpz_init(m);
      mpz_set_str(m, intStr, 10);
      std::cout << "m: " << m << std::endl;
      delete[] intStr;

      mpz_t c;
      mpz_init(c);
      // c = m^e mod n
      mpz_powm(c, m, pubKey.e, pubKey.n);
      std::cout << "c: " << c << std::endl;
      char *cStr = mpz_get_str(nullptr, 10, c);
      mpz_get_str(cStr, 10, c);
      std::cout << "cs: " << strlen(cStr) << std::endl;
      cipherText.append(cStr);   
      void (*freefunc)(void *, size_t);
      mp_get_memory_functions(nullptr, nullptr, &freefunc);
      freefunc(cStr, strlen(cStr) + 1);
      i = 0;
      blockStr.clear();
    } else {
      blockStr.push_back(*it);
      i++;
    }
  }

  return cipherText;
}

// ============================================================================
/**
* Write cipher text to file
*/
void writeCipherText(std::string& cipherText)
{
  std::string fileName("scott-axcell.ciphertext");
  std::ofstream ofs(fileName, std::ios::trunc | std::ios::out);
  if (!ofs.good()) {
    std::cerr << "ERROR: not able to write file '" << fileName << "'" << std::endl;
    exit(1);
  }

  ofs << cipherText;
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

  PublicRSAKey pubKey;
  mpz_set_str(pubKey.e, eStr.c_str(), 10); // use base 10
  mpz_set_str(pubKey.n, nStr.c_str(), 10); // use base 10

  std::cout << "Read pubKey.e: " << pubKey.e << std::endl;
  std::cout << "Read pubKey.n: " << pubKey.n << std::endl;

  std::string plainText = readFirstLineOfFile(plainTextFile);
  std::string cipherText = encryptPlainText(pubKey, plainText);
  std::cout << "PlainText: " << plainText << std::endl;
  std::cout << "CipherText: " << cipherText << std::endl;
  writeCipherText(cipherText);

  return 0;
}

