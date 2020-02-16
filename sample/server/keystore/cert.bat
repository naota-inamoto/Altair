
rem prepare ca.conf      // for openssl config of CA
rem prepare server.conf  // for openssl config of Server

rem ======== DH Param ========
mkdir dh

rem -------- step(0) -------- "openssl dsaparam" ���g���� "dh parameter" ���쐬
openssl dhparam -out dh/param.pem -dsaparam 512
rem -------- step(0.1) -------- step(0)�ō쐬���� "dh parameter" �̏ڍ�
openssl dhparam -in dh/param.pem -text

rem ======== CA ========
mkdir ca
mkdir ca\private
rem mkdir ca/private

rem -------- step(1) -------- "openssl req" ���g���� �����v�� ���쐬
openssl req -config ca.conf -out ca/req.pem -keyout ca/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� �����v�� ��verify
openssl req -config ca.conf -verify -in ca/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� �������������� �ؖ��� ���쐬
openssl x509 -md5 -CAcreateserial -in ca/req.pem -days 90 -req -out ca/cert.pem -signkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� �ؖ��� ��verify
openssl verify -CAfile ca/cert.pem ca/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� �ؖ��� �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in ca/cert.pem

rem ======== Server ========
mkdir server
mkdir server\private
rem mkdir server/private

rem -------- step(1) -------- "openssl req" ���g���� �����v�� ���쐬
openssl req -config server.conf -out server/req.pem -keyout server/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config server.conf -verify -in server/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� CA���������� �ؖ��� ���쐬
openssl x509 -md5 -CAcreateserial -in server/req.pem -days 90 -req -out server/cert.pem -CA ca/cert.pem -CAkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� �ؖ��� ��verify
openssl verify -CAfile ca/cert.pem server/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� �ؖ��� �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in server/cert.pem


rem ======== Buyer ========
mkdir buyer
mkdir buyer\private
rem mkdir buyer/private

rem -------- step(1) -------- "openssl req" ���g���� �����v�� ���쐬
openssl req -config buyer.conf -out buyer/req.pem -keyout buyer/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config buyer.conf -verify -in buyer/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� CA���������� �ؖ��� ���쐬
openssl x509 -md5 -CAcreateserial -in buyer/req.pem -days 90 -req -out buyer/cert.pem -CA ca/cert.pem -CAkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� �ؖ��� ��verify
openssl verify -CAfile ca/cert.pem buyer/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� �ؖ��� �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in buyer/cert.pem

rem ======== Supplier ========
mkdir supplier
mkdir supplier\private
rem mkdir supplier/private

rem -------- step(1) -------- "openssl req" ���g���� �����v�� ���쐬
openssl req -config supplier.conf -out supplier/req.pem -keyout supplier/private/key.pem -new
rem -------- step(1.1) -------- step(1)�ō쐬���� "certificate request" ��verify
openssl req -config supplier.conf -verify -in supplier/req.pem -noout

rem -------- step(2) -------- "openssl x509" ���g���� CA���������� �ؖ��� ���쐬
openssl x509 -md5 -CAcreateserial -in supplier/req.pem -days 90 -req -out supplier/cert.pem -CA ca/cert.pem -CAkey ca/private/key.pem
rem -------- step(2.1) -------- step(2)�ō쐬���� �ؖ��� ��verify
openssl verify -CAfile ca/cert.pem supplier/cert.pem
rem -------- step(2.2) -------- step(2)�ō쐬���� �ؖ��� �̏ڍ�
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in supplier/cert.pem



