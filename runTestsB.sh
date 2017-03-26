#!/bin/bash
#

./asymmetrickey_encr scott-axcell.publickey ../plainText.txt.large
./asymmetrickey_decr ../scott-axcell.privatekey scott-axcell.ciphertext
diff ../plainText.txt.large scott-axcell.plaintext
exit $?
