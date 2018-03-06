#include "controller.h"

/* �Q�[���{�� */
Controller::Controller()
	: scene(Scene::Ready), latest_update(SCNOW), latest_draw(SCNOW)
{
	init();
}

void Controller::init()
{
	std::map<const char*, const char*> texture_files = {
		{ "TITLE_BG", TEXTURES_DIR "title.jpg"},
		{ "Z", TEXTURES_DIR "z.png" },
		{ "OOMING", TEXTURES_DIR "ooming.png" },
		{ "STAGE_01", TEXTURES_DIR "stage_01.png" },
		{ "STAGE_02", TEXTURES_DIR "stage_02.png" },
		{ "STAGE_03", TEXTURES_DIR "stage_03.png" },
		{ "STAGE_SELECT_BG", TEXTURES_DIR "stage_select_bg.png" },
		{ "GAMEOVER_BG", TEXTURES_DIR "gameover_bg.jpg" },
		{ "GAMECLEAR_BG", TEXTURES_DIR "gameclear_bg.png" },
		{ "SELECTOR", TEXTURES_DIR "selector.png"},
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
	background = new AlwaysDisplayedPolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, textures["TITLE_BG"], INT_MAX);
	polygons.emplace_back(background);
	// �Z���N�^�̓o�^
	selector = new Selector(textures["SELECTOR"]);
	polygons.emplace_back(selector);

	// Z
	zooming_z = new ZoomingZ(textures["Z"], camera);
	polygons.emplace_back(zooming_z);

	// �X�e�[�W�p�̃T���l
	stage_thumbnails[0] = new StageThumbnail(textures["STAGE_01"], camera, SCREEN_WIDTH * 1.25f, SCREEN_HEIGHT / 2);
	stage_thumbnails[1] = new StageThumbnail(textures["STAGE_02"], camera, SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT / 2);
	stage_thumbnails[2] = new StageThumbnail(textures["STAGE_03"], camera, SCREEN_WIDTH * 1.75f, SCREEN_HEIGHT / 2);
	for (const auto& thumb : stage_thumbnails) polygons.emplace_back(thumb);

	// �V�[���؂�ւ�
	switch_scene(Scene::Title);
}

// ���C���̃��[�v�p�̏����C60fps�ŉ�ʕ`�������
void Controller::exec()
{
	update();
	draw();
}

Controller::Scene Controller::get_scene()
{
	return scene;
}

// �V�[���̐؂�ւ�
void Controller::switch_scene(Scene _scene)
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
	case Scene::Title:
		background->off();
		zooming_z->off();
		break;
	case Scene::AnimetionTitleToSelect:
		background->off();
		for (const auto& thumb : stage_thumbnails) thumb->off();
		break;
	case Scene::Select:
		background->off();
		for (const auto& thumb : stage_thumbnails) thumb->off();
		break;
	case Scene::Gaming: // �X�e�[�W���甲���ė����Ƃ��̏���
		delete stage;
		break;
	case Scene::GameOver:
		break;
	case Scene::GameClear:
		break;
	case Scene::End:
		break;
	}
	
	// �ύX��̃V�[���̏�����
	switch (_scene)
	{
	case Scene::Title:
		background->change_texture(textures["TITLE_BG"]);
		background->on();
		zooming_z->on();
		camera.x = SCREEN_WIDTH / 2;
		break;
	case Scene::AnimetionTitleToSelect:
		for (const auto& thumb : stage_thumbnails) thumb->on();
		background->on();
		break;
	case Scene::Select:
		// �w�i
		background->change_texture(textures["TITLE_BG"]);
		background->on();

		// �Z���N�^
		selector->init();
		selector->on();

		for (const auto& thumb : stage_thumbnails) thumb->on();
		
		break;
	case Scene::Gaming:
		stage = new Stage(selector->get_selection());
		break;
	case Scene::GameOver:
		// �w�i
		background->change_texture(textures["GAMEOVER_BG"]);
		background->on();
		break;
	case Scene::GameClear:
		// �w�i
		background->change_texture(textures["GAMECLEAR_BG"]);
		background->on();
		break;
	case Scene::End:
		break;
	}

	// �V�[�����������ďI���
	scene = _scene;
}

// ���C���̃A�b�v�f�[�g�����i�����������j
void Controller::update()
{
	UpdateInput();

	// xbox
	UpdateController();

	if (scene == Scene::Gaming)
	{
		game_info = stage->exec();
		switch (game_info.status)
		{
		case Stage::Status::LoadError: // ���[�h���s
			switch_scene(Scene::Select);
		case Stage::Status::Clear: // �Q�[���N���A��
			switch_scene(Scene::GameClear); // ���U���g�������Ă��ׂ������ǂƂ肠����
			break;
		case Stage::Status::Failed:  // ����������U���g��ʂ������Ă��ׂ������ǂƂ肠����
			switch_scene(Scene::GameOver);
			break;
		case Stage::Status::Retire:
			switch_scene(Scene::GameOver);
			break;
		default:
			break;
		}

	}
	else
	{
		for (const auto& polygon : polygons) polygon->update();
		auto current = SCNOW;
		switch (scene)
		{
		case Scene::Title:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::AnimetionTitleToSelect);
			}
			break;
		case Scene::AnimetionTitleToSelect:
			//  �A�j���[�V��������
			if (camera.x < SCREEN_WIDTH * 1.5f)
			{
				if (time_diff(latest_update, current) > UPDATE_INTERVAL)
				{
					latest_update = current;
					camera.x += CAMERA_MOVE_SPEED;
				}
				
			}
			else switch_scene(Scene::Select);
			
			break;
		case Scene::Select:
			// �J�[�\��������
			if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFTARROW) || GetControllerButtonTrigger(XIP_D_LEFT))
			{
				selector->left();
			}

			// �J�[�\�����E��
			if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHTARROW) || GetControllerButtonTrigger(XIP_D_RIGHT))
			{
				selector->right();
			}

			// �Q�[���J�n
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Gaming);
			}

			// �^�C�g���ɖ߂�
			if (GetKeyboardTrigger(DIK_F5) || GetControllerButtonTrigger(XIP_BACK))
			{
				switch_scene(Scene::Title);
			}
			break;
		case Scene::GameOver:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Select);
			}
			break;
		case Scene::GameClear:
			if (GetKeyboardTrigger(DIK_RETURN) || GetControllerButtonTrigger(XIP_START))
			{
				switch_scene(Scene::Select);
			}
			break;
		case Scene::End:
			break;
		}
	}	
}

// �|���S�����܂Ƃ߂ăh���[
void Controller::draw()
{
	if (scene == Scene::Gaming) return; // �Q�[������Stage->exec();�ɂ܂�����

	auto current = SCNOW;
	if (time_diff(latest_draw, current) < 1000 / FRAME_RATES) return;
	latest_draw = current;

	PolygonsVector drawing_polygons;
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