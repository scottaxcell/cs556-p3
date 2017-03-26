#include "rsa.h"

// ============================================================================
/**
* Convert integer string to equivalent char string
*/
std::string convertIntToChar(char *intStr)
{
  std::string result;
  std::cout << "creating char[] of size: " << strlen(intStr) + 3 << std::endl;
  char *tmpStr = new char[strlen(intStr) + 3];

  // Pad integer string with zeros if necesary
  int modulus = strlen(intStr) % 3;
  if (modulus == 1) {
    strcpy(tmpStr, "00");
    strcat(tmpStr, intStr);
  } else if (modulus == 2) {
    strcpy(tmpStr, "0");
    strcat(tmpStr, intStr);
  } else {
    strcpy(tmpStr, intStr);
  }

  for (int i = 0; i <= (strlen(tmpStr) - 3); i += 3) {
    int n = tmpStr[i] - '0';
    n = n * 10 + (tmpStr[i + 1] - '0');
    n = n * 10 + (tmpStr[i + 2] - '0');
    result.push_back((char)n);
  }

  delete[] tmpStr;

  return result;
}

// ============================================================================
/**
* Decrypt plaintext
*/
std::string decryptCipherText(PrivateRSAKey privKey, std::string &cipherText)
{
  std::string plainText;
  int i = 0;
  int blockSize = 1000;
  std::string blockStr;
  std::string::const_iterator it = cipherText.begin();
  std::string::const_iterator end = cipherText.end();
  for ( ; it != end; ++it) {
    if ((i == (blockSize - 1)) ||
        (std::distance(it, end) == 1)) {
      // Have a block of correct size or reached the end of the string
      blockStr.push_back(*it);
      std::cout << "block: '" << blockStr << "' size: " << blockStr.size() << std::endl;
      mpz_t m;
      mpz_init(m);
      mpz_t c;
      mpz_init(c);
      mpz_set_str(c, blockStr.c_str(), 10);
      mpz_powm(m, c, privKey.d, privKey.n);
      char *dStr = mpz_get_str(nullptr, 10, m);
      std::cout << "ds: " << strlen(dStr) << std::endl;
      plainText += convertIntToChar(dStr);
      void (*freefunc)(void *, size_t);
      mp_get_memory_functions(nullptr, nullptr, &freefunc);
      freefunc(dStr, strlen(dStr) + 1);
      i = 0;
      blockStr.clear();
    } else {
      blockStr.push_back(*it);
      i++;
    }
  }

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
  //std::cout << "PlainText: " << plainText << std::endl;
  writePlainText(plainText);

  return 0;
}

