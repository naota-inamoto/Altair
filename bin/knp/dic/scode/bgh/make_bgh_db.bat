echo off

if not exist bunruidb.txt goto error

perl bgh.pl < bunruidb.txt > bgh.dat
make_db bgh.db < bgh.dat
del bgh.dat
make_db sm2code.db < sm2code.dat

echo ���ތ�b�\�f�[�^�x�[�X���쐬����܂����B
goto end

:error
echo ���ތ�b�\�f�[�^�x�[�X�̍쐬�Ɏ��s���܂����B
goto end

:end
