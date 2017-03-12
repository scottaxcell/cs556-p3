#!/bin/bash
#

KEY1=asdnvoqweuruiueruitvzlalapq
KEY2=qpowiwqenslkansascbasdoubfsfg
PLAINTEXT=BobRoss.jpg
ENCRYPTED=scott-axcell-encrypted-str
DECRYPTED=scott-axcell-decrypted-str

./encrypt ${KEY1} ${KEY2}

./decrypt ${KEY1} ${KEY2}

diff ${PLAINTEXT} ${DECRYPTED}
