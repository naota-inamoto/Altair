
cd ..
mkdir dh
cd cert2
rem cat dh/param.pem > ../dh/param.pem
type dh\param.pem > ..\dh\param.pem

cd ..
mkdir ca
cd cert2
rem cat ca1/cert.pem > ../ca/cert.pem
type ca1\cert.pem > ..\ca\cert.pem

cd ..
mkdir server
cd server
mkdir private
cd ..
cd cert2
rem cat server/cert1.pem  > ../server/cert.pem
rem cat server/cert2.pem >> ../server/cert.pem
rem cat server/private/key.pem > ../server/private/key.pem
type server\cert1.pem > ..\server\cert.pem
type server\cert2.pem >> ..\server\cert.pem
type server\private\key.pem > ..\server\private\key.pem

cd ..
mkdir client
cd client
mkdir private
cd ..
cd cert2
rem cat client/cert1.pem > ../client/cert.pem
rem cat client/private/key.pem > ../client/private/key.pem
type client\cert1.pem > ..\client\cert.pem
type client\private\key.pem > ..\client\private\key.pem
