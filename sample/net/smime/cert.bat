
rem prepare ca.conf      // for openssl config of CA
rem prepare server.conf  // for openssl config of Server

rem ======== CA ========
mkdir ca
mkdir ca\private

rem -------- step(1) -------- "openssl req" ���g���� "certificate request" ���쐬
openssl req -config ca.conf -out ca/req.pem -keyout ca/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config ca.conf -verify -in ca/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� ��������������"certificate" ���쐬
openssl x509 -md5 -CAcreateserial -in ca/req.pem -days 90 -req -out ca/cert.pem -signkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� "certificate" ��verify
openssl verify -CAfile ca/cert.pem ca/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� "certificate" �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in ca/cert.pem

rem ======== Server ========
mkdir server
mkdir server\private

rem -------- step(1) -------- "openssl req" ���g���� "certificate request" ���쐬
openssl req -config server.conf -out server/req.pem -keyout server/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config server.conf -verify -in server/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� CA����������"certificate" ���쐬
openssl x509 -md5 -CAcreateserial -in server/req.pem -days 90 -req -out server/cert.pem -CA ca/cert.pem -CAkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� "certificate" ��verify
openssl verify -CAfile ca/cert.pem server/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� "certificate" �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in server/cert.pem

rem ======== Server2 ========
mkdir server2
mkdir server2\private

rem -------- step(1) -------- "openssl req" ���g���� "certificate request" ���쐬
openssl req -config server2.conf -out server2/req.pem -keyout server2/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config server2.conf -verify -in server2/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� CA����������"certificate" ���쐬
openssl x509 -md5 -CAcreateserial -in server2/req.pem -days 90 -req -out server2/cert.pem -CA ca/cert.pem -CAkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� "certificate" ��verify
openssl verify -CAfile ca/cert.pem server2/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� "certificate" �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in server2/cert.pem

