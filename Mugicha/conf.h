#pragma once

// ���܂�o�J
#define _CRT_SECURE_NO_WARNINGS

// �Ȃ񂿂����unless�Ȃ�
#define unless(expression) if(!(expression))
#define until(expression) while(!(expression))

// window�̃A��
#define CLASS_NAME "AppClass"
#ifndef _DEBUG
#define WINDOW_NAME "Zooming"
#else
#define WINDOW_NAME "Zooming (Debug)"
#endif

// �Œ�̐ݒ�������Ƀx�^��������

// ���\�[�X���ǂ��ɂ��邩�w�肷��
#define RESOURCES_DIR "./resources/"
#define TEXTURES_DIR RESOURCES_DIR "textures/"
#define STAGEFILES_DIR RESOURCES_DIR "stages/"
#define AUDIOS_DIR RESOURCES_DIR "audios/"

#define FRAME_RATES (60)
#define UPDATE_INTERVAL (1)

// �X�N���[���̃f�J��
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define CELL_WIDTH (75)
#define CELL_HEIGHT (75)

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