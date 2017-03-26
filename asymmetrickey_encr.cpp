#include "rsa.h"

// ============================================================================
/**
* Encrypt plaintext
*/
void encryptPlainText(PublicRSAKey pubKey, std::string &plainText)
{
  FILE *of = fopen("scott-axcell.ciphertext", "w+");

  int i = 0, j = 0;
  char tmp[4];
  int blockSize = (strlen(mpz_get_str(nullptr, 10, pubKey.n)) - 1) / 3;
  int *block = (int *)calloc(blockSize + 1, sizeof(int));
  char *cBlock = (char *)calloc(blockSize * 3 + 1, sizeof(char));
  std::string::const_iterator it = plainText.begin();
  std::string::const_iterator end = plainText.end();
  while (it != end) {
    for (i = 0; i < blockSize && it != end; i++) {
      block[i] = (char)*it;
      block[i + 1] = 0;
      it++;
    }
    for (j = 0; j < i; j++) {
      sprintf(tmp, "%d", block[j]);
      strcat(cBlock, tmp);
    }
    //std::cout << "cBlock: " << cBlock << std::endl;
    mpz_t m;
    mpz_init(m);
    mpz_set_str(m, cBlock, 10);
    if (m >= (pubKey.n - 1)) {
      std::cerr << "ERROR: m >= (n -1)" << std::endl;
    }
    mpz_t c;
    mpz_init(c);
    // c = m^e mod n
    mpz_powm(c, m, pubKey.e, pubKey.n);
    mpz_out_str(of, 10, c);
    fprintf(of, "\n");
    cBlock[0] = '\0';
  }

  fclose(of);
  free(cBlock);
  free(block);

  std::cout << "Wrote ciphertext file 'scott-axcell.ciphertext'" << std::endl;
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
  
  //std::cout << "Read eStr: " << eStr << std::endl;
  //std::cout << "Read nStr: " << nStr << std::endl;

  PublicRSAKey pubKey;
  mpz_set_str(pubKey.e, eStr.c_str(), 10); // use base 10
  mpz_set_str(pubKey.n, nStr.c_str(), 10); // use base 10

  //std::cout << "Read pubKey.e: " << pubKey.e << std::endl;
  //std::cout << "Read pubKey.n: " << pubKey.n << std::endl;

  std::string plainText = readFirstLineOfFile(plainTextFile);
  encryptPlainText(pubKey, plainText);

  return 0;
}

