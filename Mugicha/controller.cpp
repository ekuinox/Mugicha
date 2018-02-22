#include "controller.h"

/* �Q�[���{�� */
Controller::Controller()
{
	scene = Ready;
	loops = 0;
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Controller::~Controller()
{
	timeEndPeriod(1);
}

void Controller::init()
{
	std::map<const char*, const char*> texture_files = {
		{"TITLE_BG", TEXTURES_DIR "title_bg.png"},
		{"STAGE_SELECT_BG", TEXTURES_DIR "stage_select_bg.png" },
		{"SELECTOR", TEXTURES_DIR "selector.png"}
	};

	for (const auto& texture_file : texture_files)
	{
		if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file.second, &textures[texture_file.first])))
		{
#ifdef _DEBUG
			printf("�e�N�X�`���ǂݍ��ݎ��s�I");
#endif
		}
	}

	// �J�����ʒu�̏�����
	camera = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	
	// �w�i�̓o�^
	background = new Background(textures["TITLE_BG"], &camera);
	polygons.push_back(background);

	// �Z���N�^�̓o�^
	selector = new Selector(textures["SELECTOR"], &camera);
	polygons.push_back(selector);

	// �V�[���؂�ւ�
	switch_scene(Title);
}

// ���C���̃��[�v�p�̏����C60fps�ŉ�ʕ`�������
void Controller::exec()
{
	update();
	draw();
}

// �V�[���̐؂�ւ�
void Controller::switch_scene(enum scene _scene)
{
	if (scene == _scene) return; // �ω��Ȃ��Ő؂�

	// ��x�S�Ė������`�悵�Ȃ���Ԃɂ���
	for (const auto& polygon : polygons)
	{
		polygon->off();
	}

	// �ύX�O�̃V�[���̏I������
	switch (scene)
	{
	case Title:
		break;
	case Select:
		break;
	case Gaming: // �X�e�[�W���甲���ė����Ƃ��̏���
		delete stage;
		break;
	}
	
	// �ύX��̃V�[���̏�����
	switch (_scene)
	{
	case Title:
		background->change_texture(textures["TITLE_BG"]);
		background->on();
		break;
	case Select:
		// �w�i
		background->change_texture(textures["STAGE_SELECT_BG"]);
		background->on();

		// �Z���N�^
		selector->init();
		selector->on();

		break;
	case Gaming:
		stage = new Stage(selector->get_selection());
		break;
	default:
		break;
	}

	// �V�[�����������ďI���
	scene = _scene;
}

// ���C���̃A�b�v�f�[�g�����i�����������j
void Controller::update()
{
	UpdateInput();

	auto current = timeGetTime();
	if (current - latest_update < 1) return;

	if (scene != Gaming)
	{
		for (const auto& polygon : polygons)
		{
			polygon->update();
		}
	}

	switch (scene)
	{
	case Title:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(Select);
		}
		break;
	case Select:
		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFTARROW)) // �I����
		{
			selector->left();
		}

		if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHTARROW)) // �I���E
		{
			selector->right();
		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(Gaming);
		}
		break;
	case Gaming:
		auto result = stage->exec();
		switch (result.status)
		{
		case clear: // �Q�[���N���A��
			switch_scene(Title); // ���U���g�������Ă��ׂ������ǂƂ肠����
			break;
		case failed:  // ����������U���g��ʂ������Ă��ׂ������ǂƂ肠����
			switch_scene(Title);
			break;
		case retire:
			switch_scene(Title);
			break;
		default:
			break;
		}
		break;
	}
}

// �|���S�����܂Ƃ߂ăh���[
void Controller::draw()
{
	if (scene == Gaming) return; // �Q�[������Stage->exec();�ɂ܂�����

	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;

	polygon_vec drawing_polygons;
	for (const auto& polygon : polygons)
	{
		drawing_polygons.push_back(polygon);
	}

	// �傫�����̂��O�ɗ���悤��
	sort(drawing_polygons.begin(), drawing_polygons.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });

	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& drawing_polygon : drawing_polygons) drawing_polygon->draw();
	}

	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}