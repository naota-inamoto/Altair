/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _ALTAIR_STRRES_H
#define _ALTAIR_STRRES_H

#ifdef	AL_LANG_JAPANESE

#define AL_MM_FILE		AL_STR("�t�@�C��")
#define AL_MM_PROJECT	AL_STR("�v���W�F�N�g")
#define AL_MM_VIEW		AL_STR("�\��")
#define AL_MM_WINDOW	AL_STR("�E�B���h�E")
#define AL_MM_HELP		AL_STR("�w���v")
#define AL_MM_EXIT		AL_STR("�I��")
#define AL_MM_STATUSBAR AL_STR("�X�e�[�^�X�o�[")
#define AL_MM_MANUAL	AL_STR("�I�����C���}�j���A��")
#define AL_MM_VERSION	AL_STR("�o�[�W����")

#define AL_M_FILE		AL_STR("�t�@�C��")
#define AL_M_NEW		AL_STR("�V�K�쐬")
#define AL_M_SAVE		AL_STR("�㏑���ۑ�")
#define AL_M_SAVE_AS	AL_STR("���O��ς��ĕۑ�")
#define AL_M_OPEN		AL_STR("�J��")
#define AL_M_PRINT		AL_STR("����p�t�@�C��")
#define AL_M_EXIT		AL_STR("�I��")

#define AL_M_EDIT		AL_STR("�ҏW")
#define AL_M_POS		AL_STR("�}���ʒu")
#define AL_M_UPPER		AL_STR("��")
#define AL_M_LOWER		AL_STR("��")
#define AL_M_ELDER		AL_STR("�Z")
#define AL_M_BROTHER	AL_STR("��")
#define AL_M_CHILD		AL_STR("�q")
#define AL_M_UP 		AL_STR("��ֈړ�")
#define AL_M_DOWN		AL_STR("���ֈړ�")
#define AL_M_LEFT		AL_STR("���ֈړ�")
#define AL_M_RIGHT		AL_STR("�E�ֈړ�")
#define AL_M_CUT		AL_STR("�J�b�g")
#define AL_M_COPY		AL_STR("�R�s�[")
#define AL_M_PASTE		AL_STR("�\��t��")
#define AL_M_DELETE 	AL_STR("�폜")

#define AL_M_WINDOW 	AL_STR("�E�B���h�E")
#define AL_M_REDRAW 	AL_STR("�ĕ\��")
#define AL_M_PARENT 	AL_STR("�e�G�f�B�^")
#define AL_M_CLOSE		AL_STR("����")
#define AL_M_CLOSE_ALL	AL_STR("���ׂĕ���")

#define AL_M_CLASS	AL_STR("�N���X�o�^")
#define AL_M_REGIST_CLASS_LIST	AL_STR("�o�^�N���X���X�g")
#define AL_M_REGIST_CLASS	AL_STR("�o�^")
#define AL_M_UNREGIST_CLASS AL_STR("�o�^����")

#define AL_M_LIB	AL_STR("���C�u����")
#define AL_M_MKLIB	AL_STR("�쐬")
#define AL_M_MKLIB_TEXT 	AL_STR("�e�L�X�g�`��")
#define AL_M_MKLIB_GRAPH	AL_STR("�O���t�`��")
#define AL_M_MKLIB_TEXT2	AL_STR("�e�L�X�g�Q�`��")
#define AL_M_TEXT2_CLEAR	AL_STR("�N���A")
#define AL_M_TEXT2_LIST 	AL_STR("�N���X���X�g")
#define AL_M_TEXT2_ADD		AL_STR("�N���X�̒ǉ�")
#define AL_M_TEXT2_CREATE	AL_STR("�쐬")
#define AL_M_LOAD_LIB		AL_STR("���[�h")
#define AL_M_LOAD_LIB_TMP	AL_STR("�ꎞ���[�h")

#define AL_M_INTERP AL_STR("�C���^�v���^")
#define AL_M_TEST_EXEC	AL_STR("�e�X�g���s")
#define AL_M_EXEC		AL_STR("���s")

#define AL_M_MISC	AL_STR("���̑�")
#define AL_M_OPEN_WITH_OUTPUT	AL_STR("�\�[�X���o�͂��Ȃ���J��")
#define AL_M_MARK_OUTPUT	AL_STR("�A�E�g�v�b�g�Ƀ}�[�N")
#define AL_M_MEMORY_SIZE	AL_STR("�������T�C�Y")
#define AL_M_GC AL_STR("�K�[�x�[�W�R���N�V����")
#define AL_M_CLEAR_VTBL AL_STR("VTBL�A�b�v�f�[�g")

#define AL_T_PROJECT	"�v���W�F�N�g"
#define AL_T_CLASS		"�N���X  %s"
#define AL_T_MEMBER 	"�����o�[  %s"
#define AL_T_BODY		"�{�f�B  %s"
#define AL_T_OUTPUT 	"�A�E�g�v�b�g"

#define AL_GC_NEW_CELL	"������ꂽ�Z���̐� = %ld\n"
#define AL_GC_FREE_CELL "�����炠�����t���[�Z���̐� = %ld\n"
#define AL_GC_USE_CELL	"�g�p���̃Z���̐� = %ld\n"

#define AL_S001 " ) ������܂���B\n"
#define AL_S002 " . �̉E���ɗ\�����ʕ���������܂��B\n"
#define AL_S003 " . �̍��ӂ��N���X�̃I�u�W�F�N�g�ł͂���܂���B\n"
#define AL_S004 " . �̍��ӂ̓��X�g�ł��B\n"
#define AL_S005 " :: �̌�ɃN���X�̃����o�[��������܂���B\n"
#define AL_S006 " ; ������܂���B\n"
#define AL_S007 " ; �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S008 " ? �ɑΉ����� : ������܂���B\n"
#define AL_S009 " break �̌�� ; ������܂���B\n"
#define AL_S010 " break ���̍Ō�� ; �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S011 " continue �̌�� ; ������܂���B\n"
#define AL_S012 " continue ���̍Ō�� ; �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S013 " else �̌�� { ������܂���B\n"
#define AL_S014 " else �̌�� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S015 " if ���̍ŏ��� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S016 " if ���̏������� ( ������܂���B\n"
#define AL_S017 " if ���̏������� ) ������܂���B\n"
#define AL_S018 " if ���̏������̌�� { ������܂���B\n"
#define AL_S019 " loop �̌�� { ������܂���B\n"
#define AL_S020 " loop ���̍ŏ��� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S021 " new �̌�ɃN���X��������܂���B\n"
#define AL_S022 " para �̌�Ɋ֐���������܂���B\n"
#define AL_S023 " para ���� ; ������܂���B\n"
#define AL_S024 " para ���̍Ō�� ; �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S025 " return ���� ; ������܂���B\n"
#define AL_S026 " return ���ɖ߂�l������܂���B\n"
#define AL_S027 " return ���̍Ō�� ; �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S028 " runtime �̌�ɃN���X��������܂���B\n"
#define AL_S029 " } �ɑΉ����� { ������܂���B\n"
#define AL_S030 " } �ɑΉ����� ������ {, if, loop ������܂���B\n"
#define AL_S031 " } �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S032 " �����̍Ō�� } �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S033 " �����̍ŏ��� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S034 "%s �̓I�u�W�F�N�g�̃����o�[�ł͂���܂���B\n"
#define AL_S035 "%s �̓N���X���ł͂���܂���B\n"
#define AL_S036 "%s �͌^���ł͂���܂���B\n"
#define AL_S037 "%s �͎��ʎq�Ƃ��ĔF���ł��܂���B\n"
#define AL_S038 "%s �v���g�^�C�v�Ɉ�v���郁���o�[�֐�������܂���B\n"
#define AL_S039 "%s::%s �͎��ʎq�Ƃ��ĔF���ł��܂���B\n"
#define AL_S040 "DeleteLine�ŃG���[���N���܂����B\n"
#define AL_S041 "GUIparts�m�[�h���s���ł�\n"

#define AL_S043 "GUI�m�[�h���s���ł�\n"
#define AL_S044 "InsertLine�ŃG���[���N���܂����B\n"
#define AL_S045 "public:, protected:, private:������܂���B\n"

#define AL_S047 "ReplaceLine�ŃG���[���N���܂����B\n"
#define AL_S048 "static �������͌^��������܂���B\n"
#define AL_S049 "Translate:: �֐��̒��g����`����Ă��܂���B\n"
#define AL_S050 "void �̓��[�J���ϐ��̌^�Ƃ��Ďg���܂���B\n"
#define AL_S051 "���̃N���X�������͂��̔h���N���X���O���Q�Ƃ��Ă���̂ŃO���t�`���̃��C�u�����ɂ͂ł��܂���B\n"
#define AL_S052 "���̃N���X�������͂��̔h���N���X���Q�Ƃ���Ă��邽�ߍ폜�ł��܂���B\n"
#define AL_S053 "���̃v���W�F�N�g�� %ld �o�C�g�̃��������v���O�����Ɏg�p���Ă��܂��B\n"
#define AL_S054 "���̃����o�[�͎Q�Ƃ���Ă���̂Ńv���g�^�C�v��ύX�ł��܂���B\n"
#define AL_S055 "���ꂪ�ŉ��ʂ̃N���X�ł��B\n"
#define AL_S056 "���ꂪ�ŉ��ʂ̃v���W�F�N�g�ł��B\n"
#define AL_S057 "���ꂪ�ŉ��ʂ̃����o�[�ł��B\n"
#define AL_S058 "���ꂪ�ŏ�ʂ̃N���X�ł��B\n"
#define AL_S059 "���ꂪ�ŏ�ʂ̃v���W�F�N�g�ł��B\n"
#define AL_S060 "���ꂪ�ŏ�ʂ̃����o�[�ł��B\n"
#define AL_S061 "�C���^�v���^���s�ŃG���[���N���܂����B�i %s �j\n"
#define AL_S062 "�I�u�W�F�N�g�|�[�g�̃\�[�X�Ɍ^��������܂���B\n"
#define AL_S063 "�J�[�\���̂���ꏊ�ƍs��}������ꏊ���Ⴂ�܂��B\n"
#define AL_S064 "�J�[�\���� %ld�s�ځA�}���ꏊ�� %ld�s�ڂł��B\n"
#define AL_S065 "�J�b�g�o�b�t�@���I�[�v���ł��܂���B\n"
#define AL_S066 "�L���X�g���� ) ������܂���B\n"
#define AL_S067 "�N���X %s ���v���W�F�N�g�ɓo�^����܂����B\n"
#define AL_S068 "�N���X %s ����`����Ă��܂���B\n"
#define AL_S069 "�N���X %s �̓v���W�F�N�g����o�^��������܂����B\n"
#define AL_S070 "�N���X %s �̓v���W�F�N�g�ɓo�^����Ă��邽�ߍ폜�ł��܂���B\n"
#define AL_S071 "�N���X %s �͊��ɓo�^����Ă��܂��B\n"
#define AL_S072 "�N���X %s �͓o�^����Ă��܂���B\n"
#define AL_S073 "�N���XGraph��������܂���B\n"
#define AL_S074 "�N���XGUIbuilder��������܂���B\n"
#define AL_S075 "�N���X�̃��X�g\n"
#define AL_S076 "�N���X���w�肵�Ă��������B\n"
#define AL_S077 "�N���X��I�����Ă��������B\n"
#define AL_S078 "�N���X��������܂���B\n"
#define AL_S079 "�N���X�����N���X��\�킵�Ă��܂���B\n"
#define AL_S080 "�N���X����������ɂȂ��Ă��܂���B\n"
#define AL_S081 "�N���X���̌�� :: ������܂���B\n"
#define AL_S082 "�O���t�`�����C�u���� %s ���쐬����܂����B(%s)\n"
#define AL_S083 "�O���t�`�����C�u���� %s ���쐬�� . . . (%s)\n"
#define AL_S084 "�R�����g���ɗ\������EOF������܂��B\n"
#define AL_S085 "�R���g���[���o�̓|�[�g��������܂���B\n"
#define AL_S086 "�R���g���[�����̒u�������͂ł��܂���B\n"
#define AL_S087 "�V�X�e���֐� %s �̈������̐��������Ă��܂���B\n"
#define AL_S088 "�e�L�X�g�`�����C�u���� %s ���쐬������܂����B(%s)\n"
#define AL_S089 "�e�L�X�g�`�����C�u���� %s ���쐬�� . . . (%s)\n"
#define AL_S090 "�e���|�����t�@�C���ɕ��@�G���[������܂��B\n"
#define AL_S091 "�e���|�����t�@�C���ɗ\������EOF������܂��B\n"
#define AL_S092 "�f�[�^�o�̓|�[�g�̃f�X�g���I�u�W�F�N�g�m�[�h�ł���܂���B\n"
#define AL_S093 "�f�[�^�o�̓|�[�g�̃f�X�g�Ɍ^����������܂���B\n"
#define AL_S094 "�f�[�^�o�̓|�[�g�悪�I�u�W�F�N�g�m�[�h�ł���܂���B\n"
#define AL_S095 "�f�[�^���̓|�[�g��������܂���B\n"
#define AL_S096 "�f�[�^���̓|�[�g�̃\�[�X�Ɍ^��������܂���B\n"

#define AL_S099 "�g�����X���[�g������ɏI�����܂����B\n"
#define AL_S100 "�g�����X���[�g���ɃG���[���������܂����B\n"
#define AL_S101 "�m�[�h���֐��m�[�h�ł���܂���B\n"
#define AL_S102 "�m�[�h���֐��m�[�h�ł��}�N���m�[�h�ł�����܂���B\n"
#define AL_S103 "�m�[�h����65535���z����O���t�̓R�s�[�ł��܂���B\n"
#define AL_S104 "�t�@�C�� %s ���I�[�v���ł��܂���B\n"
#define AL_S105 "�t�@�C�� %s ���I�[�v���ł��܂���ł����B\n"
#define AL_S106 "�t�@�C�� %s ��ǂݍ��ݒ��ɃG���[���������܂����B\n"
#define AL_S107 "�t�@�C�����I�[�v���ł��܂���B\n"
#define AL_S108 "�t�@�C�������݂��܂���B\n"
#define AL_S109 "�t�@�C���̃I�[�v���Ɏ��s���܂����B\n"
#define AL_S110 "�t�@�C���������݂ŃG���[���������܂����B\n"
#define AL_S111 "�t�@�C���ǂݍ��ݒ��ɃG���[���������܂����B\n"
#define AL_S112 "�v���W�F�N�g���I�[�v������܂����B\n"
#define AL_S113 "�v���W�F�N�g��I�����Ă��������B\n"
#define AL_S114 "�v���W�F�N�g�N���X�����[�h�� . . .\n"
#define AL_S115 "�x�[�X�ƂȂ�N���X %s ����`����Ă��܂���B\n"
#define AL_S116 "�x�[�X�ƂȂ�N���X��������܂���B\n"
#define AL_S117 "�x�[�X�ƂȂ�N���X����`����Ă��܂���B\n"

#define AL_S119 "�}�N���ɓ����ăR���g���[���A�[�N�����ǂ�Ȃ��Ȃ�܂����B\n"

#define AL_S121 "�����o�[��I�����Ă��������B\n"
#define AL_S122 "�����o�[�ϐ��̌���const�ɂȂ��Ă��܂��B\n"
#define AL_S123 "�����o�[�ϐ�����������܂���B\n"
#define AL_S124 "�����o�[��������܂���B\n"
#define AL_S125 "���C�u���� %s ��������܂���B\n"
#define AL_S126 "���C�u���� %s �����[�h�� . . .\n"
#define AL_S127 "���[�g�N���X AlObject �̓��C�u�������ł��܂���B\n"
#define AL_S128 "���[�g�N���X AlObject �͍폜�ł��܂���B\n"
#define AL_S129 "���[�J���錾�̍Ō�� ; ������܂���B\n"
#define AL_S130 "���[�J���ϐ��� { } �̊Ԃł����g���܂���B\n"
#define AL_S131 "���[�J���ϐ��錾�Ō^��������܂���B\n"
#define AL_S132 "���[�J���ϐ����錾�Ɍ�肪����܂��B\n"
#define AL_S133 "����ŃG���[���������܂����B\n"
#define AL_S134 "�������Ȃ���public�N���X�֐����炵���e�X�g���s�ł��܂���B\n"
#define AL_S135 "�������Ȃ���public�N���X�֐����炵�����s�ł��܂���B\n"
#define AL_S136 "�������̌^�� %s �͐錾����Ă��܂���B\n"
#define AL_S137 "�������̌^��������܂���B\n"
#define AL_S138 "��������������܂���B\n"
#define AL_S139 "����������������܂���B\n"
#define AL_S140 "���ʎ��̕����� ) ������܂���B\n"
#define AL_S141 "�֐� %s �̈������ɊԈႢ������܂��B\n"
#define AL_S142 "�֐� %s �̓v���g�^�C�v�Ɉ�v����֐�������܂���B\n"
#define AL_S143 "�֐�Graph::SrcNode��������܂���B\n"
#define AL_S144 "�֐�GUIbuilder::GetParts��������܂���B\n"
#define AL_S145 "�֐�GUIbuilder::LoadAndCreateWindow��������܂���B\n"
#define AL_S146 "�֐��̃x�[�X�N���X��������܂���B\n"
#define AL_S147 "�֐��������̊֐���������܂���B\n"
#define AL_S148 "�֐��������̍Ō�� ) ������܂���B\n"
#define AL_S149 "�֐����������X�g�̍Ō�� ) ������܂���B\n"
#define AL_S150 "�֐����������X�g�̍ŏ��� ( ������܂���B\n"
#define AL_S151 "�֐���`��������܂���B\n"
#define AL_S152 "�֐���`��������܂���B�����炭�������̌^���Ԉ���Ă��܂��B\n"
#define AL_S153 "���ɂ��郉�C�u�������D�悳��܂��B\n"
#define AL_S154 "���ɃI�[�v������Ă��܂��B\n"
#define AL_S155 "�^�� %s �͐錾����Ă��܂���B\n"
#define AL_S156 "�^��������܂���B\n"
#define AL_S157 "�ŏ��̃T�u�N���X�Ȃ̂ŉE�ֈړ��ł��܂���B\n"

#define AL_S159 "�q�N���X��I�����Ă��������B\n"
#define AL_S160 "���̍Ō�� ; ������܂���B\n"
#define AL_S161 "�o�̓|�[�g�̃f�X�g�Ƀ����o�[�ϐ�����������܂���B\n"
#define AL_S162 "�[���̂��󂢐��䕶�ɂ܂������č폜���邱�Ƃ͂ł��܂���B\n"
#define AL_S163 "�ΏۃI�u�W�F�N�g���萔�ɂȂ��Ă��܂�\n"
#define AL_S164 "������̍��ӂ����Ӓl�ł���܂���B\n"
#define AL_S165 "��`����Ă��Ȃ�const�̃f�[�^�ł��B\n"
#define AL_S166 "��`����Ă��Ȃ�list�̃f�[�^�ł��B\n"
#define AL_S167 "�萔���Ɍ�肪����܂��B\n"
#define AL_S168 "�����N���X�������݂��܂��B\n"
#define AL_S169 "�����v���ƃ^�C�v���������֐������ɂ���܂��B\n"
#define AL_S170 "�����v���g�^�C�v�̃����o�[�����ɂ���܂��B\n"
#define AL_S171 "�������O�̊֐�������܂��B\n"
#define AL_S172 "�������O�̕ϐ�������܂��B\n"
#define AL_S173 "���ꖼ�̃v���W�F�N�g�����݂��܂��B\n"
#define AL_S174 "���ꖼ�̃��C�u���������Ƀ��[�h����Ă��܂��B\n"
#define AL_S175 "��������܂���B\n"
#define AL_S176 "���� * �̋߂��ŕ��@�G���[������܂��B\n"
#define AL_S177 "���@���Ԉ���Ă��܂��B\n"
#define AL_S178 "���@�G���[������܂��B\n"
#define AL_S179 "����`�̃t�@�C���t�H�[�}�b�g�ł��B\n"
#define AL_S180 "AlObject�̖��O�͕ς����܂���B\n"
#define AL_S181 "�N���X�o�^����Ă���N���X�̖��O�͕ς����܂���B\n"
#define AL_S200 "�e�L�X�g�Q�`�����C�u���� %s ���쐬������܂����B\n"
#define AL_S201 "�e�L�X�g�Q�`�����C�u���� %s ���쐬�� . . .\n"
#define AL_S202 "�N���X�֐��|�C���^��NULL�ł��B\n"
#define AL_S203 "�\���͈������Ƃ��Ďg���܂���B"
#define AL_S204 "�\���͕ϐ����Ƃ��Ďg���܂���B"
#define AL_S205 "�\���̓����o�[���Ƃ��Ďg���܂���B"
#define AL_S206 "static�֐�����this�͎g���܂���B"
#define AL_S207 "static�֐�����static�łȂ������o�[�֐��͎g���܂���B"
#define AL_S208 "static�֐�����static�łȂ������o�[�ϐ��͎g���܂���B"
#define AL_S209 "'%s'�́Aprotected: �܂��� private: �ŉB������Ă��܂��B"

#define AL_S210 "Windows3.1�ł�64K�𒴂���T�C�Y�̃r�b�g�}�b�v�͓ǂݍ��߂܂���B\n"
#define AL_S211 "�\�[�X���o�͂��Ȃ��烍�[�h���郂�[�h�ɂȂ�܂����B"
#define AL_S212 "�\�[�X���o�͂��Ȃ��Ń��[�h���郂�[�h�ɂȂ�܂����B"

#define AL_S400 " try ���� { ������܂���B\n"
#define AL_S401 " try ���� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S402 " catch �̌�� ( ������܂���B\n"
#define AL_S403 " catch �̌�� ( �̌�ɗ�O�N���X��������܂���B\n"
#define AL_S404 " catch �̌�̗�O�N���X���̌�ɕϐ���������܂���B\n"
#define AL_S405 " catch �̌�̕ϐ����̌�� ) ������܂���B\n"
#define AL_S406 " catch �̌�� ) �̌�� { ������܂���B\n"
#define AL_S407 " catch �̌�� { �̌�ɗ]�v�ȕ���������܂��B\n"
#define AL_S408 " throw �̌�Ɏ�������܂���B\n"
#define AL_S409 " throw �̌�̎��̌�� ; ������܂���B\n"
#define AL_S410 " throw �̌�� ; �̌�ɗ]�v�ȕ���������܂��B\n"

#define AL_O1	"OLE�̏������Ɏ��s���܂����B\n"
#define AL_O2	"%s: �I�[�g���[�V����������32�܂łł��B"
#define AL_O3	"%s: �I�[�g���[�V�����̑Ώۂ�IDispatch�ł���܂���B"
#define AL_O4	"%s: �I�[�g���[�V�������\�b�h��������܂���B"
#define AL_O5	"%s: OLE��O���������܂����B"
#define AL_O6	"%s: OLE�f�B�X�p�b�`��O���������܂����B"
#define AL_O7	"%s: ��������O���������܂����B"
#define AL_O8	"%s: �I�[�g���[�V�����v���p�e�B��������܂���B"
#define AL_O9	"OLE�I�[�g���[�V�����̓T�|�[�g����Ă��܂���B\n"
#define AL_O10	"%s: IDispatch��NULL�ł��B"

#endif	/* AL_LANG_JAPANESE */

#ifdef	AL_LANG_ENGLISH

#define AL_MM_FILE		AL_STR("File")
#define AL_MM_PROJECT	AL_STR("Project")
#define AL_MM_VIEW		AL_STR("View")
#define AL_MM_WINDOW	AL_STR("Window")
#define AL_MM_HELP		AL_STR("Help")
#define AL_MM_EXIT		AL_STR("Exit")
#define AL_MM_STATUSBAR AL_STR("StatsuBar")
#define AL_MM_MANUAL	AL_STR("Online Manual")
#define AL_MM_VERSION	AL_STR("Version")

#define AL_M_FILE		AL_STR("File")
#define AL_M_NEW		AL_STR("New")
#define AL_M_SAVE		AL_STR("Save")
#define AL_M_SAVE_AS	AL_STR("Save As")
#define AL_M_OPEN		AL_STR("Open")
#define AL_M_PRINT		AL_STR("File for Print")
#define AL_M_EXIT		AL_STR("Exit")

#define AL_M_EDIT		AL_STR("Edit")
#define AL_M_POS		AL_STR("Insert Position")
#define AL_M_UPPER		AL_STR("Upper")
#define AL_M_LOWER		AL_STR("Lower")
#define AL_M_ELDER		AL_STR("Elder Brother")
#define AL_M_BROTHER	AL_STR("Brother")
#define AL_M_CHILD		AL_STR("Child")
#define AL_M_UP 		AL_STR("Move Up")
#define AL_M_DOWN		AL_STR("Move Down")
#define AL_M_LEFT		AL_STR("Move Left")
#define AL_M_RIGHT		AL_STR("Move Right")
#define AL_M_CUT		AL_STR("Cut")
#define AL_M_COPY		AL_STR("Copy")
#define AL_M_PASTE		AL_STR("Paste")
#define AL_M_DELETE 	AL_STR("Delete")

#define AL_M_WINDOW 	AL_STR("Window")
#define AL_M_REDRAW 	AL_STR("Redraw")
#define AL_M_PARENT 	AL_STR("Parent Editor")
#define AL_M_CLOSE		AL_STR("Close")
#define AL_M_CLOSE_ALL	AL_STR("Close All")

#define AL_M_CLASS	AL_STR("Class Registration")
#define AL_M_REGIST_CLASS_LIST	AL_STR("Registrated Class List")
#define AL_M_REGIST_CLASS	AL_STR("Regist Class")
#define AL_M_UNREGIST_CLASS AL_STR("Unregist Class")

#define AL_M_LIB	AL_STR("Library")
#define AL_M_MKLIB	AL_STR("Create")
#define AL_M_MKLIB_TEXT 	AL_STR("Text Format")
#define AL_M_MKLIB_GRAPH	AL_STR("Graph Format")
#define AL_M_MKLIB_TEXT2	AL_STR("Text2 Format")
#define AL_M_TEXT2_CLEAR	AL_STR("Clear")
#define AL_M_TEXT2_LIST 	AL_STR("Class List")
#define AL_M_TEXT2_ADD		AL_STR("Add Class")
#define AL_M_TEXT2_CREATE	AL_STR("Create")
#define AL_M_LOAD_LIB		AL_STR("Load")
#define AL_M_LOAD_LIB_TMP	AL_STR("Temp Load")

#define AL_M_INTERP AL_STR("Interpreter")
#define AL_M_TEST_EXEC	AL_STR("Test Execution")
#define AL_M_EXEC		AL_STR("Execution")

#define AL_M_MISC	AL_STR("Others")
#define AL_M_OPEN_WITH_OUTPUT	AL_STR("Open width Soruce Output")
#define AL_M_MARK_OUTPUT	AL_STR("Mark Output")
#define AL_M_MEMORY_SIZE	AL_STR("Momory Size")
#define AL_M_GC AL_STR("Garbage Collection")
#define AL_M_CLEAR_VTBL AL_STR("update VTBL")

#define AL_T_PROJECT	"Project"
#define AL_T_CLASS		"Class  %s"
#define AL_T_MEMBER 	"Member  %s"
#define AL_T_BODY		"Body  %s"
#define AL_T_OUTPUT 	"Output"

#define AL_GC_NEW_CELL	"number of collected cells = %ld\n"
#define AL_GC_FREE_CELL "number of free cells = %ld\n"
#define AL_GC_USE_CELL	"number of cells in use = %ld\n"

#define AL_FONT "System"

#define AL_S001 " expected ')' .\n"
#define AL_S002 " unexpected character on right hand side of '.' .\n"
#define AL_S003 " left hand side of '.' is not instance of class.\n"
#define AL_S004 " left hand side of '.' ls list.\n"
#define AL_S005 " after '::' member name of class is missing.\n"
#define AL_S006 " expected ';' .\n"
#define AL_S007 " unexpected character after ';' .\n"
#define AL_S008 " expected ':' corresponding to '?' .\n"
#define AL_S009 " expected ';' after 'break'.\n"
#define AL_S010 " unexpected character after 'break'.\n"
#define AL_S011 " expected ';' after 'continue'.\n"
#define AL_S012 " unexpected character after 'continue'.\n"
#define AL_S013 " expected '{' corresponding to 'else'."
#define AL_S014 " unexpected character after '} else {'.\n"
#define AL_S015 " unexpected character after 'if (expression) {'.\n"
#define AL_S016 " expected '(' for condition of if statement.\n"
#define AL_S017 " expected ')' for condition of if statement.\n"
#define AL_S018 " expected '{' for if statement.\n"
#define AL_S019 " expected '{' after 'loop'.\n"
#define AL_S020 " unexpected character after 'loop {'.\n"
#define AL_S021 " there is no class name after 'new'.\n"
#define AL_S022 " there is no function expression after 'para'.\n"
#define AL_S023 " expected ';' after 'para' statement.\n"
#define AL_S024 " unexpected character after 'para' statement.\n"
#define AL_S025 " ';' is missing in 'return' statement.\n"
#define AL_S026 " return value is missing in 'return' statement.\n"
#define AL_S027 " unexpected character after 'return' statement.\n"
#define AL_S028 " class name is missing after 'runtime'.\n"
#define AL_S029 " expected '}' corresponding to '{'.\n"
#define AL_S030 " '{', 'if' and 'loop' are missing which correspond to '}'.\n"
#define AL_S031 " unexpected character after '}'.\n"
#define AL_S032 " unexpected character after '}'.\n"
#define AL_S033 " unexpected character after '{'.\n"
#define AL_S034 "%s is not a member of object.\n"
#define AL_S035 "%s is not a class name.\n"
#define AL_S036 "%s is not a type name\n"
#define AL_S037 "%s can not be recognized as an identifier.\n"
#define AL_S038 "function %s does not match any prototype.\n"
#define AL_S039 "%s::%s can not be recognized as an identifier.\n"
#define AL_S040 "error occurres in DeleteLine.\n"
#define AL_S041 "GUIparts node is illegal.\n"

#define AL_S043 "GUI node is illegal.\n"
#define AL_S044 "error occurres in InsertLine.\n"
#define AL_S045 "no 'public:', 'protected:', 'private:'.\n"

#define AL_S047 "error occurred in ReplaceLine.\n"
#define AL_S048 "'static' or type name is missing.\n"
#define AL_S049 "Translate:: function is not defined.\n"
#define AL_S050 "void can not be used as a type for local variable.\n"
#define AL_S051 "can't make this class library because this class or its derived class have outside reference.\n"
#define AL_S052 "can't delete this class because this class is refered to.\n"
#define AL_S053 "this project uses %d bytes memory for program.\n"
#define AL_S054 "can't change prototype of this member because it is refered to.\n"
#define AL_S055 "this class is the lowest.\n"
#define AL_S056 "this project is the lowest.\n"
#define AL_S057 "this member is the lowest\n"
#define AL_S058 "this class is the highest\n"
#define AL_S059 "this project is the highest.\n"
#define AL_S060 "this member is the highest.\n"
#define AL_S061 "interpreter error occurred. (%s)\n"
#define AL_S062 "source of object port does not have a type name.\n"
#define AL_S063 "caret position and insert position is defferent.\n"
#define AL_S064 "caret is at the line %ld, insert position is al the line %ld.\n"
#define AL_S065 "can't open cutbuffer.\n"
#define AL_S066 "expected ')' at the case expression.\n"
#define AL_S067 "class %s is registrated into the project.\n"
#define AL_S068 "class %s is not defined.\n"
#define AL_S069 "class %s is unregistrated from the project.\n"
#define AL_S070 "can't delete class %s because it is registrated.\n"
#define AL_S071 "class %s is already registrated.\n"
#define AL_S072 "class %s is not registrated.\n"
#define AL_S073 "can't find class Graph.\n"
#define AL_S074 "can't find class GUIbuilder.\n"
#define AL_S075 "List of Classes\n"
#define AL_S076 "please specify class.\n"
#define AL_S077 "please select class.\n"
#define AL_S078 "no class name.\n"
#define AL_S079 "this name is not a class name.\n"
#define AL_S080 "argument representing class is not string.\n"
#define AL_S081 "expected '::' after class name.\n"
#define AL_S082 "graph-format library %s is created. (%s)\n"
#define AL_S083 "creating graph-format library %s . . . (%s)\n"
#define AL_S084 "unexpected EOF in comment statement.\n"
#define AL_S085 "can't find output control port.\n"
#define AL_S086 "can't replace control statement.\n"
#define AL_S087 "number of arguments system function %s is not matched.\n"
#define AL_S088 "text-format library %s is created. (%s)\n"
#define AL_S089 "creating text-format library %s  . . . (%s)\n"
#define AL_S090 "there is syntax error in temporary file.\n"
#define AL_S091 "unexpected EOF in temporary file.\n"
#define AL_S092 "destination of output data port is not object node.\n"
#define AL_S093 "can't find type name in destination of output data port.\n"
#define AL_S094 "destination of output data port is not object node.\n"
#define AL_S095 "can't find input data port.\n"
#define AL_S096 "can't find type name in soruce of input data port.\n"

#define AL_S099 "translation is succeded.\n"
#define AL_S100 "translation if failed.\n"
#define AL_S101 "node is not function node.\n"
#define AL_S102 "node is not function node nor macro node\n"
#define AL_S103 "can't copy graph if number of nodes of the graph is grater than 65535. \n"
#define AL_S104 "can't open file %s.\n"
#define AL_S105 "can't open file %s.\n"
#define AL_S106 "file read error occurred at the  file %s.\n"
#define AL_S107 "can't open file.\n"
#define AL_S108 "file does not exist.\n"
#define AL_S109 "can't open file.\n"
#define AL_S110 "file write error occurred.\n"
#define AL_S111 "file read error occurred.\n"
#define AL_S112 "the project is opend.\n"
#define AL_S113 "please select project.\n"
#define AL_S114 "loading project classes . . .\n"
#define AL_S115 "base-class %s is not defined.\n"
#define AL_S116 "can't find base-class.\n"
#define AL_S117 "base-class is not defined.\n"

#define AL_S119 "can't traversal control arc in macro.\n"

#define AL_S121 "please select mamber.\n"
#define AL_S122 "root of member variable is const.\n"
#define AL_S123 "can't find member variable.\n"
#define AL_S124 "can't find member name.\n"
#define AL_S125 "can't find library %s.\n"
#define AL_S126 "loading library %s . . .\n"
#define AL_S127 "can't make root Class AlObject library.\n"
#define AL_S128 "can't delete root class AlObject.\n"
#define AL_S129 "expected ';' after local variable declaration.\n"
#define AL_S130 "local variables can be used only between '{' and '}'.\n"
#define AL_S131 "expected type name in local variable declaration.\n"
#define AL_S132 "there is syntax error in local variable declaration.\n"
#define AL_S133 "print error occurred.\n"
#define AL_S134 "only public class-function without arguments invokes test-execution.\n"
#define AL_S135 "only public class-function without arguments invokes execution.\n"
#define AL_S136 "argument type name %s is not defined.\n"
#define AL_S137 "no argument type name.\n"
#define AL_S138 "no argument name.\n"
#define AL_S139 "can't find argument name.\n"
#define AL_S140 "expected ')' corresponding to '('.\n"
#define AL_S141 "there is an error in armugent of function %s.\n"
#define AL_S142 "there is no function that prototypes is same with function %s.\n"
#define AL_S143 "can't find function Graph::SrcNode.\n"
#define AL_S144 "can't find function GUIbuilder::GetParts.\n"
#define AL_S145 "can't find function GUIbuilder::LoadAndCreateWindow.\n"
#define AL_S146 "can't find base-class of function.\n"
#define AL_S147 "can't find function.\n"
#define AL_S148 "expected ')' for last of function argument list.\n"
#define AL_S149 "expected ')' for last of function argument list.\n"
#define AL_S150 "expected '(' for the first of function argument list.\n"
#define AL_S151 "can't find function definition.\n"
#define AL_S152 "can't find function deficition. argument type will be incorrect.\n"
#define AL_S153 "first loaded library is used.\n"
#define AL_S154 "already opened.\n"
#define AL_S155 "type name %s is not defined.\n"
#define AL_S156 "no type name.\n"
#define AL_S157 "can't move right because it is first subclass.\n"

#define AL_S159 "please select child class.\n"
#define AL_S160 "expected ';' last of expression.\n"
#define AL_S161 "can't find member variable in destination of output port.\n"
#define AL_S162 "can't delete statements over the deferent depth.\n"
#define AL_S163 "object is constant.\n"
#define AL_S164 "left hand side of assignment statement is not LHS value.\n"
#define AL_S165 "undefined const data.\n"
#define AL_S166 "undefined list data.\n"
#define AL_S167 "there is an error in constant expression.\n"
#define AL_S168 "there is a class that name is same with.\n"
#define AL_S169 "there is a function that prototype is same with\n"
#define AL_S170 "there is a function that prototype is same with.\n"
#define AL_S171 "there is a function that name is same with.\n"
#define AL_S172 "there is a variable that name is same with.\n"
#define AL_S173 "there is a project that name is same with.\n"
#define AL_S174 "same name library is already loaded.\n"
#define AL_S175 "no statement.\n"
#define AL_S176 "there is syntax error near '*'.\n"
#define AL_S177 "syntax error.\n"
#define AL_S178 "syntax error.\n"
#define AL_S179 "undefined file format.\n"
#define AL_S180 "class name of AlObject can't be changed.\n"
#define AL_S181 "name of registered class can't be changed.\n"
#define AL_S200 "text2-format library %s is created.\n"
#define AL_S201 "creating text2-format library %s  . . .\n"
#define AL_S202 "class function pointer is null.\n"
#define AL_S203 "preserved identifier cannot be used as an argument name."
#define AL_S204 "preserved identifier cannot be used as a variable name."
#define AL_S205 "preserved identifier cannot be used as a member name."
#define AL_S206 "'this' cannot be used in static functions."
#define AL_S207 "not-static-member function cannot be used in static functions."
#define AL_S208 "not-static-member variable cannot be used in static functions."
#define AL_S209 "'%s' is concealed by 'protected:' or 'private:'"
#define AL_S210 "In Windows3.1, can't read bitmap size of which is greater than 64K.\n"
#define AL_S211 "load mode with source output."
#define AL_S212 "load mode without source output."

#define AL_S400 " '{' missing after 'try'.\n"
#define AL_S401 " unexpected token after 'try {'.\n"
#define AL_S402 " '(' missing after 'catch'.\n"
#define AL_S403 " Exception classname missing after 'catch ('.\n"
#define AL_S404 " variable name missing after Exception classname.\n"
#define AL_S405 " ')' missing after variable name.\n"
#define AL_S406 " '{' missing after ')'.\n"
#define AL_S407 " unexpected token after '{'.\n"
#define AL_S408 " expession missing after 'throw'.\n"
#define AL_S409 " ';' missing after expression.\n"
#define AL_S410 " unexpected token after ';'\n"

#define AL_O1	"fail to initialize OLE.\n"
#define AL_O2	"%s: maximun size of arguments is 32."
#define AL_O3	"%s: autometion object is not IDispatch."
#define AL_O4	"%s: can't find autometion method."
#define AL_O5	"%s: COleException occurs."
#define AL_O6	"%s: COleDispatchException occurs."
#define AL_O7	"%s: CMemoryException occurs."
#define AL_O8	"%s: can7t find automation property."
#define AL_O9	"OLE automation is not supported.\n"
#define AL_O10	"%s: IDispatch is NULL."

#endif	/* AL_LANG_ENGLISH */

#endif
