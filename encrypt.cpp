#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <bitset>
#include <cstdint>
#include <limits>

#define BYTE char
#define KEYSIZE 10
#define BYTE_LIMIT std::numeric_limits<BYTE>::max()


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

  ifs.seekg(0, std::ios::end);
  std::streampos fileSize = ifs.tellg();
  std::vector<BYTE> result(fileSize);

  ifs.seekg(0, std::ios::beg);
  ifs.read(result.data(), fileSize);

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
  ///*DEBUG*/std::cout << "DEBUG  original: " << &plainText[0] << std::endl;
  ///*DEBUG*/std::cout << "DEBUG encrypted: " << &c1[0] << std::endl;
  ///*DEBUG*/std::cout << "DEBUG decrypted: " << &p1[0] << std::endl;
  if (plainText != p1) {
    std::cerr << "FAILURE: vigenere cipher failed" << std::endl;
  } else {
    std::cerr << "SUCCESS: vigenere cipher success" << std::endl;
  }

}

std::vector<std::vector<BYTE>> createMatrix(std::vector<BYTE> &data)
{
  std::vector<std::vector<BYTE>> matrix;
  std::vector<BYTE> v1;
  if (data.size() < KEYSIZE) {
    //std::cout << "data < KEYSIZE\n";
    for (auto &c : data)
      v1.push_back(c);
    matrix.push_back(v1);
  } else {
    for (auto &c : data) {
      v1.push_back(c);
      //std::cout << "Adding column element " << v1.size() << std::endl;
      if (v1.size() == KEYSIZE) {
        //std::cout << "Adding row\n";
        matrix.push_back(v1);
        v1.clear();
      }
    }
    if (v1.size())
      matrix.push_back(v1);
  }

  // TODO not sure if xFF will work. maybe use pointers instead with nullptr?
  std::vector<BYTE> &lastRow = matrix.back();
  int numToFill = KEYSIZE - lastRow.size();
  if (numToFill != 0) {
    //std::cout << "PADDING last row in 2D vector\n";
    for (int i = 0; i < numToFill; i++) {
      //std::cout << "added xFF\n";
      lastRow.push_back(BYTE_LIMIT);
    }
  }

  return matrix;
}

std::vector<int> getTraversalOrder(std::vector<BYTE> &key)
{
  std::vector<BYTE> sortedKey(key);
  std::sort(sortedKey.begin(), sortedKey.end());
  //std::cout << "       key: " << key.size() << ": " << &key[0] << std::endl;
  //std::cout << "sorted key: " << sortedKey.size() << ": " << &sortedKey[0] << std::endl;
  std::vector<int> traversalOrder;
  for (auto &sk : sortedKey) {
    auto it = std::find(key.begin(), key.end(), sk);
    if (it != key.end()) {
      traversalOrder.push_back(std::distance(key.begin(), it));
    } else {
      std::cerr << "ERROR!\n";
    }
  }
  //std::cout << "traverse order key: ";
  //for (auto &i : traversalOrder) {
  //  std::cout << i << ",";
  //}
  //std::cout << std::endl;

  return traversalOrder;
}

std::vector<std::vector<BYTE>> createOrderedMatrix(std::vector<int> &traversalOrder, std::vector<BYTE> &cipherText)
{
  int numRows = cipherText.size() / KEYSIZE;
  std::vector<std::vector<BYTE>> matrix(numRows, std::vector<BYTE>(KEYSIZE, BYTE_LIMIT));

  int rowIdx = 0;
  int *colIdx = &traversalOrder[0]; 
  for (auto &c : cipherText) {
    if (rowIdx == numRows) {
      rowIdx = 0;
      colIdx++;
    }
    matrix[rowIdx][*colIdx] = c;
    rowIdx++;
  }

  //for (int i = 0; i < matrix.size(); i++) {
  //  for (int j = 0; j < KEYSIZE; j++) {
  //    std::cout << matrix[i][j];
  //  }
  //  std::cout << std::endl;
  //}

  return matrix;
}

std::vector<BYTE> createCipherText(std::vector<int> &traversalOrder, std::vector<std::vector<BYTE>> &matrix)
{
  std::vector<BYTE> cipherText;
  for (auto &colIdx : traversalOrder) {
    //std::cout << "Column " << colIdx << std::endl;
    for (int j = 0; j < matrix.size(); j++) {
      //std::cout << matrix[j][colIdx] << std::endl;
      cipherText.push_back(matrix[j][colIdx]);
    }
  }

  return cipherText;
}

std::vector<BYTE> decryptCipherText(std::vector<std::vector<BYTE>> &matrix)
{
  std::vector<BYTE> cipherText;
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      cipherText.push_back(matrix[i][j]);
    }
  }

  return cipherText;
}

// ============================================================================
/**
* Double transposition cipher (decryption)
*/
std::vector<BYTE> decryptTranspositionCipher(std::vector<BYTE> &c1,
                                             std::vector<BYTE> &key1, 
                                             std::vector<BYTE> &key2)
{
  // Decrypt C3 to C2(D1)
  std::vector<int> keyOrder2 = getTraversalOrder(key2);
  std::vector<std::vector<BYTE>> m3(createOrderedMatrix(keyOrder2, c1));
  std::vector<BYTE> d1(decryptCipherText(m3));
  //std::cout << "D1: " << d1.size() << ": " << &d1[0] << std::endl;;

  // Decrypt C2 to C1(D2)
  std::vector<int> keyOrder1(getTraversalOrder(key1));
  std::vector<std::vector<BYTE>> m4(createOrderedMatrix(keyOrder1, d1));
  std::vector<BYTE> d2(decryptCipherText(m4));
  //std::cout << "D2: " << d2.size() << ": " << &d2[0] << std::endl;;

  auto ri = d2.rbegin();
  auto rend = d2.rend();
  for (; ri != rend; ++ri) {
    if (*ri == BYTE_LIMIT)
      d2.pop_back();
    else
      break;
  }
  return d2;

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
  std::vector<std::vector<BYTE>> m1(createMatrix(c1));

  //std::cout << "C1: " << c1.size() << ": " << &c1[0] << std::endl;;

  // Figure out which order to traverse columns
  std::vector<int> keyOrder1(getTraversalOrder(key1));

  // Create C2 by iterating over consecutive columns based on the sorted keys 
  std::vector<BYTE> c2(createCipherText(keyOrder1, m1));
  //std::cout << "C2: " << c2.size() << ": " << &c2[0] << std::endl;;

  // Create C3 by iterating over consecutive columns based on the sorted keys 
  std::vector<std::vector<BYTE>> m2(createMatrix(c2));
  std::vector<int> keyOrder2 = getTraversalOrder(key2);
  std::vector<BYTE> c3(createCipherText(keyOrder2, m2));
  //std::cout << "C3: " << c3.size() << ": " << &c3[0] << std::endl;;

  // Decrypt C3 to C2(D1)
  std::vector<std::vector<BYTE>> m3(createOrderedMatrix(keyOrder2, c3));
  std::vector<BYTE> d1(decryptCipherText(m3));
  //std::cout << "D1: " << d1.size() << ": " << &d1[0] << std::endl;;

  if (d1 != c2) {
    std::cerr << "ERROR: transposition from second matrix to first cipher failed\n";
    std::cout << "C2: " << c2.size() << ": " << &c2[0] << std::endl;;
    std::cout << "D1: " << d1.size() << ": " << &d1[0] << std::endl;;
  }

  // Decrypt C2 to C1(D2)
  std::vector<std::vector<BYTE>> m4(createOrderedMatrix(keyOrder1, d1));
  std::vector<BYTE> d2(decryptCipherText(m4));
  //std::cout << "D2: " << d2.size() << ": " << &d2[0] << std::endl;;

  int numToRemove = d2.size() - c1.size();
  if (numToRemove > 0) {
    for (int i = 0; i < numToRemove; i++)
      d2.pop_back();
  }
  if (d2 != c1) {
    std::cerr << "ERROR: transposition from first matrix to vegenere cipher failed\n";
    std::cout << "C1: " << c1.size() << ": " << &c1[0] << std::endl;;
    std::cout << "D2: " << d2.size() << ": " << &d2[0] << std::endl;;
  }

  return c3;
}

std::string getFileName()
{
  std::string fileName;
  std::cout << "Please provide binary file to be encrypted: ";
  std::getline(std::cin, fileName);
  return fileName;
}

// ============================================================================
/**
* Write data to binary file
*/
void writeOutput(std::vector<BYTE> &data, std::string &fileName)
{
  std::ofstream ofs(fileName, std::ios::trunc | std::ios::binary | std::ios::out);
  if (!ofs.good()) {
    std::cerr << "ERROR: not able to write file '" << fileName << "'" << std::endl;
    exit(1);
  }

  ofs.write((const char*)&data[0], data.size());
  ofs.close();

  std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
  std::streampos fileSize;
  ifs.seekg(0, std::ios::end);
  fileSize = ifs.tellg();
  ifs.close();

  std::cout << "Wrote " << fileSize << " bytes to ciphertext '" << fileName << "'" << std::endl;
}

// ============================================================================
/**
* Main
*/
int main(int argc, char *argv[])
{
  std::cout << "/* =======" << std::endl;
  std::cout << "/* Encrypt" << std::endl;
  std::cout << "/* =======" << std::endl;
  if (argc != 3) {
    std::cerr << "USAGE: ./encrypt <key_1> <key_2>" << std::endl;
    return 1;
  }    

  std::string keyStr1(argv[1]);
  std::string keyStr2(argv[2]);
  std::string encryptedFile("scott-axcell-encrypted-str");
  std::string decryptedFile("scott-axcell-decrypted-str");

  // ==========================================================================
  /**
  * Read Key1
  */
  std::vector<BYTE> key1 = uniquifyKey(keyStr1);
  std::cout << "Using Key 1: " << std::string(key1.begin(), key1.end()) << std::endl;

  // ==========================================================================
  /**
  * Read Key2
  */
  std::vector<BYTE> key2 = uniquifyKey(keyStr2);
  std::cout << "Using Key 2: " << std::string(key2.begin(), key2.end()) << std::endl;

  // ==========================================================================
  /**
  * Get binary file name
  */
  // TODO
  std::string fileName = getFileName();
 
  // ==========================================================================
  /**
  * Read binary file into memory
  */
  std::vector<BYTE> plainText(readBinaryFile(fileName));
  std::cout << "Read " << plainText.size() << " bytes from plaintext '" << fileName << "'" << std::endl;

  // ==========================================================================
  /**
  * Tests
  */
  //tests(fileName, key1, key2);
  
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

  /*DEBUG*/
  //writeOutput(c3, encryptedFile);

  //std::cout << "Expected C3: " << c3.size() << ": " << &c3[0] << std::endl;;
  //c3 = readBinaryFile(encryptedFile);
  //std::cout << "Read     C3: " << c3.size() << ": " << &c3[0] << std::endl;;

  //std::vector<BYTE> d1(decryptTranspositionCipher(c3, key1, key2));

  //std::vector<BYTE> p1(vigenereCipher(d1, key1));

  //std::cout << "P0: " << plainText.size() << ": " << &plainText[0] << std::endl;;
  //std::cout << "P1: " << p1.size() << ": " << &p1[0] << std::endl;;

  //writeOutput(p1, decryptedFile);
  /*DEBUG END*/

  // ==========================================================================
  /**
  * Write encrypted file
  */
  writeOutput(c3, encryptedFile);

  return 0;
}
