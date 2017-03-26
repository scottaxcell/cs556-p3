#!/bin/bash
#

KEY1=asdnvoqweuruiueruitvzlalapq
KEY2=qpowiwqenslkansascbasdoubfsfg
PLAINTEXT=BobRoss.jpg
ENCRYPTED=scott-axcell-encrypted-str
DECRYPTED=scott-axcell-decrypted-str

#./encrypt ${KEY1} ${KEY2}
#
#./decrypt ${KEY1} ${KEY2}
#
#diff ${PLAINTEXT} ${DECRYPTED}
./genRSAKey
./asymmetrickey_encr pub.pem plainText.txt.large
./asymmetrickey_decr priv.pem scott-axcell.ciphertext
diff plainText.txt.large scott-axcell.plaintext
