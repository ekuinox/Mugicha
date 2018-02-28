#pragma once

#define _WITHOUT_DEATH

// ���܂�o�J
#define _CRT_SECURE_NO_WARNINGS

// �Ȃ񂿂����unless�Ȃ�
#define unless(expression) if(!(expression))
#define until(expression) while(!(expression))

// window�̃A��
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Zooming"

// �Œ�̐ݒ�������Ƀx�^��������

// ���\�[�X���ǂ��ɂ��邩�w�肷��
#define RESOURCES_DIR "./resources/"
#define TEXTURES_DIR RESOURCES_DIR "textures/"
#define STAGEFILES_DIR RESOURCES_DIR "stages/"

#define FRAME_RATES (60)
#define UPDATE_INTERVAL (1)

// �X�N���[���̃f�J��
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define CELL_WIDTH (100)
#define CELL_HEIGHT (100)

// �v���C���̃f�J��
#define PLAYER_SPEED (0.675f) // �v���C��ړ����x
#define PLAYER_JUMP_POWER (2.0f) // �W�����v�́C1ms�ɂǂ񂾂���Ԃ�
#define PLAYER_JUMP_TIME (500) // �W�����v����(ms)
#define PLAYER_FALLING (1.0f) // �������x

// �w�i�̃f�J��
#define BACKGROUND_WIDTH (SCREEN_WIDTH)
#define BACKGROUND_HEIGHT (SCREEN_HEIGHT)
#define BACKGROUND_X (BACKGROUND_WIDTH / 2)
#define BACKGROUND_Y (BACKGROUND_HEIGHT / 2)

// �J�����ʒu => �悭�킩��񂯂ǃv���C���ɒǏ]����悤�l����C����͏����ʒu
// �e�|���S����vertexes�𐶐�����Ƃ��ɎQ�Ƃ���C���������
#define CAMERA_X (0)
#define CAMERA_Y (0)

#define B_TO_S(exp) exp ? "TRUE" : "FALSE"