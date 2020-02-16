
rem  ˆÃ†‰»
openssl smime -encrypt -rc2-40 -in message.txt   -out message_e.txt  server/cert.pem

rem •¡‡‰»
openssl smime -decrypt -rc2-40 -in message_e.txt -out message_d.txt  -recip server/cert.pem -inkey server/private/key.pem

rem ----------------------------------------------------------------------------------------------

rem –¼
openssl smime -sign -in message.txt -out message_s.txt -nocerts -signer server/cert.pem -inkey server/private/key.pem

rem –¼ŒŸØ

openssl smime -verify -in message_s.txt -certfile server/cert.pem  -CAfile ca/cert.pem


