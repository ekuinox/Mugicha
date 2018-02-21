#pragma once

#define unless(expression) if(!(expression))

/* Constant */
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Zooming"


// �Œ�̐ݒ�������Ƀx�^��������

// �X�N���[���̃f�J��
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (800)

// �v���C���̃f�J��
#define PLAYER_WIDTH (50)
#define PLAYER_HEIGHT (50)
#define PLAYER_X (300)
#define PLAYER_Y (50)

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