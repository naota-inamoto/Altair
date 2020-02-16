
rem prepare root_ca.conf  // for openssl config of rootCA
rem prepare ca1.conf      // for openssl config of CA1
rem prepare ca2.conf      // for openssl config of CA2
rem prepare server.conf   // for openssl config of Server
rem prepare client.conf   // for openssl config of Client

rem ======== DH Param ========
mkdir dh

rem -------- step(0) -------- "openssl dsaparam" を使って "dh parameter" を作成
openssl dhparam -out dh/param.pem -dsaparam 512
rem -------- step(0.1) -------- step(0)で作成した "dh parameter" の詳細
openssl dhparam -in dh/param.pem -text

rem ======== rootCA ========
mkdir root_ca
cd root_ca
mkdir private
cd ..

rem -------- step(1) -------- "openssl req" を使って 署名要求 を作成
openssl req -config root_ca.conf -out root_ca/req.pem -keyout root_ca/private/key.pem -new
rem -------- step(1.1) -------- step(1)で作成した 署名要求 のverify
openssl req -config root_ca.conf -verify -in root_ca/req.pem -noout

rem -------- step(2) -------- "openssl x509" を使って 自分が署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in root_ca/req.pem -days 90 -req -out root_ca/cert.pem -signkey root_ca/private/key.pem
rem -------- step(2.1) -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile root_ca/cert.pem root_ca/cert.pem
rem -------- step(2.2) -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in root_ca/cert.pem

rem ======== CA1 ========
mkdir ca1
cd ca1
mkdir private
cd ..

rem -------- step(1) -------- "openssl req" を使って 署名要求 を作成
openssl req -config ca1.conf -out ca1/req.pem -keyout ca1/private/key.pem -new
rem -------- step(1.1) -------- step(1)で作成した "certificate request" のverify
openssl req -config ca1.conf -verify -in ca1/req.pem -noout

rem -------- step(2) -------- "openssl x509" を使って rootCAが署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in ca1/req.pem -days 90 -req -out ca1/cert.pem -CA root_ca/cert.pem -CAkey root_ca/private/key.pem
rem -------- step(2.0.1) -------- 中間CAの証明書にルートCAの証明書追加
rem cat root_ca/cert.pem >> ca1/cert.pem
type root_ca\cert.pem >> ca1\cert.pem
rem -------- step(2.1) -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile root_ca/cert.pem ca1/cert.pem
rem -------- step(2.2) -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in ca1/cert.pem

rem ======== CA2 ========
mkdir ca2
cd ca2
mkdir private
cd ..

rem -------- step(1) -------- "openssl req" を使って 署名要求 を作成
openssl req -config ca2.conf -out ca2/req.pem -keyout ca2/private/key.pem -new
rem -------- step(1.1) -------- step(1)で作成した "certificate request" のverify
openssl req -config ca2.conf -verify -in ca2/req.pem -noout

rem -------- step(2) -------- "openssl x509" を使って rootCAが署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in ca2/req.pem -days 90 -req -out ca2/cert.pem -CA root_ca/cert.pem -CAkey root_ca/private/key.pem
rem -------- step(2.0.1) -------- 中間CAの証明書にルートCAの証明書追加
rem cat root_ca/cert.pem >> ca2/cert.pem
type root_ca\cert.pem >> ca2\cert.pem
rem -------- step(2.1) -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile root_ca/cert.pem ca2/cert.pem
rem -------- step(2.2) -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in ca2/cert.pem

rem ======== Server ========
mkdir server
cd server
mkdir private
cd ..

rem -------- step(1) -------- "openssl req" を使って 署名要求 を作成
openssl req -config server.conf -out server/req.pem -keyout server/private/key.pem -new
rem -------- step(1.1) -------- step(1)で作成した "certificate request" のverify
openssl req -config server.conf -verify -in server/req.pem -noout

rem -------- step(2) -------- "openssl x509" を使って CA1が署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in server/req.pem -days 90 -req -out server/cert1.pem -CA ca1/cert.pem -CAkey ca1/private/key.pem
rem -------- step(2.1) -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile ca1/cert.pem server/cert1.pem
rem -------- step(2.2) -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in server/cert1.pem

rem -------- step(2') -------- "openssl x509" を使って CA2が署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in server/req.pem -days 90 -req -out server/cert2.pem -CA ca2/cert.pem -CAkey ca2/private/key.pem
rem -------- step(2.1') -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile ca2/cert.pem server/cert2.pem
rem -------- step(2.2') -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in server/cert2.pem

rem ======== Client ========
mkdir client
cd client
mkdir private
cd ..

rem -------- step(1) -------- "openssl req" を使って 署名要求 を作成
openssl req -config client.conf -out client/req.pem -keyout client/private/key.pem -new
rem -------- step(1.1) -------- step(1)で作成した "certificate request" のverify
openssl req -config client.conf -verify -in client/req.pem -noout

rem -------- step(2) -------- "openssl x509" を使って CA1が署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in client/req.pem -days 90 -req -out client/cert1.pem -CA ca1/cert.pem -CAkey ca1/private/key.pem
rem -------- step(2.1) -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile ca1/cert.pem client/cert1.pem
rem -------- step(2.2) -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in client/cert1.pem

rem -------- step(2') -------- "openssl x509" を使って CA2が署名した 証明書 を作成
openssl x509 -md5 -CAcreateserial -in client/req.pem -days 90 -req -out client/cert2.pem -CA ca2/cert.pem -CAkey ca2/private/key.pem
rem -------- step(2.1') -------- step(2)で作成した 証明書 のverify
openssl verify -CAfile ca2/cert.pem client/cert2.pem
rem -------- step(2.2') -------- step(2)で作成した 証明書 の詳細
openssl x509 -md5 -subject -issuer -startdate -enddate -noout -in client/cert2.pem

