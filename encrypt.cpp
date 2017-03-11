#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <bitset>

#define BYTE unsigned char
#define BITS std::bitset<8>
#define BITSETSIZE 8
#define KEYSIZE 10


// ============================================================================
/**
* Uniquify a key string
*/
std::vector<BYTE> uniquifyKey(std::string key)
{
  if (key.size() < KEYSIZE) {
    std::cerr << "ERROR: Key '" << key << "' is too small, please provide a larger key." << std::endl;
    exit(1);
  }

  std::vector<BYTE> uniqueKey;
  std::locale loc;
  for (int i = 0; i < key.size(); i++) {
    if (uniqueKey.size() == KEYSIZE)
      break;
    BYTE c = std::toupper(key[i], loc);
    if (std::find(uniqueKey.begin(), uniqueKey.end(), c) == uniqueKey.end())
      uniqueKey.push_back(c);
  }

  if (uniqueKey.size() < KEYSIZE) {
    std::cerr << "ERROR: Key '" << key << "' is too small, please provide a larger key." << std::endl;
    exit(1);
  }

  return uniqueKey;
}

std::vector<std::bitset<8>>
bytesToBitsets(std::vector<BYTE> &bytes)
{
  std::vector<std::bitset<8>> bits;
  bits.reserve(bytes.size());
  for (auto &byte : bytes) {
    bits.push_back(std::bitset<8>(byte));
  }
  return bits;
}

std::vector<BYTE>
bitsetsToBytes(std::vector<std::bitset<8>> &bits)
{
  std::vector<BYTE> bytes;
  bits.reserve(bits.size());
  for (auto &bitset : bits) {
    unsigned long l = bitset.to_ulong(); 
    BYTE c = static_cast<BYTE>(l);
    bytes.push_back(c);
  }
  return bytes;
}

// ============================================================================
/**
* Read binary file
*/
std::vector<BYTE> readBinaryFile(std::string fileName)
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

  std::vector<BYTE> result;
  result.reserve(fileSize);
  result.insert(result.begin(),
             std::istream_iterator<BYTE>(ifs),
             std::istream_iterator<BYTE>());

  return result;
}


// ============================================================================
/**
* Vigenere cipher
*/
std::vector<BYTE>
vigenereCipher(std::vector<BYTE> &plainText, std::vector<BYTE> &key)
{
  std::vector<BYTE> ciphertext;
  std::locale loc;

  int j = 0;
  for (int i = 0; i < plainText.size(); i++) {
    BYTE p = plainText[i];
    BYTE k = key[j];
    ciphertext.push_back((p ^ k));
    j = (j + 1) % key.size();
  }

  return ciphertext;
}

// ============================================================================
/**
* Vigenere cipher
*/
void tests(std::string fileName, std::vector<BYTE> key1, std::vector<BYTE> key2)
{
  //
  // BINARY FILE READ
  // 
  std::vector<BYTE> plainText = readBinaryFile(fileName);
  /*DEBUG*/std::cout << "DEBUG Read " << plainText.size() << " bytes from " << fileName << std::endl;

  //
  // VIGENERE
  // 
  std::vector<BYTE> c1(vigenereCipher(plainText, key1));
  std::vector<BYTE> p1(vigenereCipher(c1, key1));
  /*DEBUG*/std::cout << "DEBUG  original: " << &plainText[0] << std::endl;
  /*DEBUG*/std::cout << "DEBUG encrypted: " << &c1[0] << std::endl;
  /*DEBUG*/std::cout << "DEBUG decrypted: " << &p1[0] << std::endl;
  if (plainText != p1) {
    std::cerr << "FAILURE: vigenere cipher failed" << std::endl;
  } else {
    std::cerr << "SUCCESS: vigenere cipher success" << std::endl;
  }

  //
  // BYTES/BITSETS
  // 
  std::vector<std::bitset<8>> bitsets = bytesToBitsets(plainText);
  std::vector<BYTE> bytes = bitsetsToBytes(bitsets);
  if (bytes != plainText) {
    std::cout << "FAILUE: bytes/bitsets converion failed" << std::endl;
  } else {
    std::cout << "SUCCESS: bytes/bitsets converion success" << std::endl;
  }
}

// ============================================================================
/**
* Double transposition cipher (encryption)
*/
std::vector<BYTE> doubleTranspositionCipher(std::vector<BYTE> &c1,
                                            std::vector<BYTE> &key1, 
                                            std::vector<BYTE> &key2)
{
  // Fill 2D vector with c1
  //std::vector<BYTE> cipher1;
  //std::string str("helloMyFriend");
  //for (auto &s : str) {
  //  cipher1.push_back(reinterpret_cast<BYTE>(s));
  //} 
  std::vector<std::vector<BYTE>> twoD;
  int numBytes = c1.size();
  int i = 0, j =0;
  std::vector<BYTE> v1;

  for (auto &c : c1) {
  //for (int i = 0; i < numBytes; i++) {
    v1.push_back(c);
    //std::cout << "Adding column element " << v1.size() << std::endl;
    if (v1.size() == KEYSIZE) {
      //std::cout << "Adding row\n";
      twoD.push_back(v1);
      v1.clear();
    }
  }

  std::cout << "2D VECTOR" << std::endl;
  for (int i = 0; i < twoD.size(); i++) {
    for (int j = 0; j < KEYSIZE; j++) {
      std::cout << twoD[i][j];
    }
    std::cout << std::endl;
  }

  int rowIdx = 2;
  std::cout << "Row " << rowIdx  << std::endl;
  int rowSize = twoD[rowIdx].size();
  for (int i = 0; i < rowSize; i++) {
    std::cout << twoD[rowIdx][i] << std::endl;
  }

  int colIdx = 8;
  std::cout << "Column " << colIdx  << std::endl;
  for (int j = 0; j < twoD.size(); j++) {
    std::cout << twoD[j][colIdx] << std::endl;
  }

  // TODO sort key in order
  // TODO iterate on consecutive columns creating groups of 5
  // TODO push_back the groups of 5 to into a vector

}

// ============================================================================
/**
* Main
*/
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

  std::string keyStr1(argv[1]);
  std::string keyStr2(argv[2]);
  std::string fileName(argv[3]);
  std::string encryptedFile("scott-axcell-encrypted-str");
  std::string decryptedFile("scott-axcell-decrypted-str");

  // ==========================================================================
  /**
  * Read Key1
  */
  std::vector<BYTE> key1 = uniquifyKey(keyStr1);
  /*DEBUG*/std::cout << "DEBUG Key 1: " << &key1[0] << std::endl;

  // ==========================================================================
  /**
  * Read Key2
  */
  std::vector<BYTE> key2 = uniquifyKey(keyStr2);
  /*DEBUG*/std::cout << "DEBUG Key 2: " << &key2[0] << std::endl;

  // ==========================================================================
  /**
  * Get binary file name
  */
  // TODO
  //std::string fileName = getFileName();
 
  // ==========================================================================
  /**
  * Read binary file into memory
  */
  std::vector<BYTE> plainText = readBinaryFile(fileName);
  /*DEBUG*/std::cout << "DEBUG Read " << plainText.size() << " bytes from " << fileName << std::endl;

  // ==========================================================================
  /**
  * Tests
  */
  tests(fileName, key1, key2);
  
  // ==========================================================================
  /**
  * Generate C1 using Vigenere cipher
  */
  std::vector<BYTE> c1(vigenereCipher(plainText, key1));

  // ==========================================================================
  /**
  * Generate C3 using double transposition cipher
  */
  std::vector<BYTE> c3(doubleTranspositionCipher(c1, key1, key2));

  // ==========================================================================
  /**
  * Write encrypted file
  */
  // TODO
  //void writeOutput(c3, encryptedFile);

  return 0;
}
