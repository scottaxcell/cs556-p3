#!/bin/bash
#

KEY1=CORNFLAKES
KEY2=BLACKHORSE
PLAINTEXT=large.binary
ENCRYPTED=scott-axcell-encrypted-str
DECRYPTED=scott-axcell-decrypted-str

./encrypt ${KEY1} ${KEY2} ${PLAINTEXT}

#./decrypt ${KEY1} ${KEY2} ${ENCRYPTED}

diff ${PLAINTEXT} ${DECRYPTED}
