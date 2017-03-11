#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>

#define BYTE char
#define KEYSIZE 10

std::string uniquifyKey(std::string key)
{
  if (key.size() < KEYSIZE) {
    std::cerr << "ERROR: Key '" << key << "' is too small, please provide a larger key." << std::endl;
    exit(1);
  }

  std::string chars;
  std::locale loc;
  for (int i = 0; i < key.size(); i++) {
    if (chars.size() == KEYSIZE)
      break;
    BYTE c = std::toupper(key[i], loc);
    if (std::find(chars.begin(), chars.end(), c) == chars.end())
      chars.push_back(c);
  }

  if (chars.size() < KEYSIZE) {
    std::cerr << "ERROR: Key '" << key << "' is too small, please provide a larger key." << std::endl;
    exit(1);
  }

  return chars;
}

std::string readBinaryFile(std::string fileName)
{
  std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
  if (!ifs.good()) {
    std::cerr << "ERROR: not able to open file '" << fileName << "'" << std::endl;
    exit(1);
  }
  std::streampos fileSize;

  ifs.seekg(0, std::ios::end);
  fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  std::string result;
  result.reserve(fileSize);
  result.insert(result.begin(),
             std::istream_iterator<BYTE>(ifs),
             std::istream_iterator<BYTE>());

  return result;
}

std::string
encryptVigenere(std::string fileName,
                std::string &key)
{
  std::string ciphertext;
  std::locale loc;

  std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
  if (!ifs.good()) {
    std::cerr << "ERROR: not able to open file '" << fileName << "'" << std::endl;
    exit(1);
  }
  std::streampos fileSize;

  ifs.seekg(0, std::ios::end);
  fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  
  int  j = 0;
  // Read binary file, 10 bytes (80 bits) at a time
  std::streampos totalBytesRead = 0;
  while (totalBytesRead < fileSize) {
    std::string buffer;
    buffer.reserve(KEYSIZE);
    ifs.read(reinterpret_cast<char*>(&buffer[0]), (KEYSIZE*sizeof(char)));
    std::streamsize bytesRead = ifs.gcount();
    totalBytesRead += bytesRead;
    /*DEBUG*/std::cout << "DEBUG read " << bytesRead << " " << &buffer[0] << std::endl;
    /*DEBUG*/std::cout << "DEBUG read total of " << totalBytesRead << " of " << fileSize << " from file\n";

    for (int i = 0; i < bytesRead; i++) {
      BYTE byte = std::toupper(buffer[i], loc);
      if (!std::isupper(byte, loc))
        continue;
      char cipher = ((byte - 'A') + (key[j] - 'A')) % 26 + 'A';
      char decrypted = (cipher - key[j] + 26) % 26 + 'A';
      std::cout << "key{" << j << "] " << byte << "->" << cipher << "->" << decrypted << std::endl;
      ciphertext.push_back(cipher);
      j = (j + 1) % key.size();
    }
  }

  return ciphertext;
}

std::string
decryptVigenere(std::string &encrypted,
                std::string &key)
{
  std::string plaintext;
  std::locale loc;

  int j = 0;
  for (int i = 0; i < encrypted.size(); i++) {
    BYTE byte = std::toupper(encrypted[i], loc);
    if (!std::isupper(byte, loc))
      continue;
    char decrypted = (byte - key[j] + 26) % 26 + 'A';
    std::cout << "key[" << j << "] " << byte << "->" << decrypted << std::endl;
    plaintext.push_back(decrypted);
    j = (j + 1) % key.size();
  }

  return plaintext;
}

int main(int argc, char *argv[])
{
  // TODO only read keys then ask for binary file
  //if (argc != 3) {
  //  std::cerr << "USAGE: ./encrypt <key_1> <key_2>" << std::endl;
  //  return 1;
  //}    
  if (argc != 4) {
    std::cerr << "USAGE: ./encrypt <key_1> <key_2> <binary_file>" << std::endl;
    return 1;
  }    

  std::string key1 = uniquifyKey(std::string(argv[1]));
  /*DEBUG*/std::cout << "DEBUG Key 1: " << &key1[0] << std::endl;

  std::string key2 = uniquifyKey(std::string(argv[2]));
  /*DEBUG*/std::cout << "DEBUG Key 2: " << &key2[0] << std::endl;

  std::string msg = readBinaryFile(std::string(argv[3]));
  /*DEBUG*/std::cout << "DEBUG Read " << msg.size() << " bytes from " << std::string(argv[3]) << std::endl;

  std::string encrypted = encryptVigenere(std::string(argv[3]), key1);
  std::string decrypted = decryptVigenere(encrypted, key1);
  /*DEBUG*/std::cout << "DEBUG  original: " << &msg[0] << std::endl;
  /*DEBUG*/std::cout << "DEBUG encrypted: " << &encrypted[0] << std::endl;
  /*DEBUG*/std::cout << "DEBUG decrypted: " << &decrypted[0] << std::endl;

  if (decrypted != msg) {
    std::cout << "Cipher failed\n";
  } else {
    std::cout << "Cipher success!\n";
  }

  return 0;
}
