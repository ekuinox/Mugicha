#include "stage.h"
#include "collision_checker.h"

// �R���X�g���N�^
Stage::Stage()
{
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	status = prep;

	init();
}

Stage::Stage(char _stage_select)
{
	// _stage_select�ɂ���ēǂݍ��ރX�e�[�W����؂�ւ�����
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	status = prep;

//	init();
	if (_stage_select == 1) init1();
	if (_stage_select == 2) init2();
	if (_stage_select == 3) init3();

}

// �f�X�g���N�^
Stage::~Stage()
{
	for (auto tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}
}

// ������@���ăX�e�[�W���X�V������`�悵���肷��
enum status Stage::exec()
{
	update();
	draw();
	return status;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second, &textures[_texture.first]); // ��ēǂݍ���
	}
}

// ������, �R���X�g���N�^����Ă�
void Stage::init()
{
	if (textures.empty())
	{
		multi_texture_loader({
//			{ "__TAG__", "__FILE_PATH__" },
			{ "BACKGROUND", "./resources/textures/background.jpg" },
			{ "PLAYER", "./resources/textures/player.jpg" },
			{ "BLOCK", "./resources/textures/block.png" },
			{ "ORIGIN", "./resources/textures/origin.png" },
			{ "BLOCK2", "./resources/textures/block2.png" },
			{ "SAMPLE1", "./resources/textures/sample1.png"},
			});
	}

	std::map<enum PolygonTypes, polygon_vec>().swap(polygons); // ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���

	// �w�i�̓o�^
	background = push_polygon_back(BACKGROUND, new Background(textures["BACKGROUND"], &camera));
	background->enable();
	background->show();

	// �v���C���̓o�^
	player = push_polygon_back(PLAYER, new Player(textures["PLAYER"], &camera, polygons));
	player->enable();
	player->show();

	// �g�k�ł���I�u�W�F�N�g��o�^
	polygons[SCALABLE_OBJECT].push_back(new ScalableObject(25, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons[SCALABLE_OBJECT].back()->enable();
	polygons[SCALABLE_OBJECT].back()->show();
	
	// ���_
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// x���K�C�h���C��
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// y���K�C�h���C��
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	zoom_level = { 1, 1 };
}

// �X�V����
void Stage::update()
{
	// ���Ԃ��C�ɂ��Ȃ�����

	// �^�C�g���ɖ߂�i���m�F�j
	if (GetKeyboardTrigger(DIK_1))
	{
		status = end;
		return;
	}

	// �g�k
	if (GetKeyboardTrigger(DIK_O) && zoom_level.h < 2.0f) // �g��
	{
		zoom_level.w *= 2;
		zoom_level.h *= 2;
#ifdef _DEBUG
		printf("zoom_level: (%f, %f)\n", zoom_level.w, zoom_level.h);
#endif
	}
	else if (GetKeyboardTrigger(DIK_L) && zoom_level.w > 0.5f) // �k��
	{
		zoom_level.w /= 2;
		zoom_level.h /= 2;
#ifdef _DEBUG
		printf("zoom_level: (%f, %f)\n", zoom_level.w, zoom_level.h);
#endif
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
#ifdef _DEBUG
		std::cout << "Trigger SPACE" << std::endl;
#endif
		static_cast<Player*>(polygons[PLAYER][0])->jump();
	}


	// ���Ԃ��C�ɂ������
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

	// �S�Y�[���ύX
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->zoom(zoom_level);
	}

	// ��������X�V����
	camera.x = polygons[PLAYER][0]->get_coords().x;
	camera.y = polygons[PLAYER][0]->get_coords().y + 200; // �v���C������̃J�����̍����C��������Ȃ񂩕ς��Ǝv��
	
	// ��ʊO�͌����Ȃ��悤�ɂ���
	if (camera.x < BACKGROUND_X) camera.x = BACKGROUND_X;
	if (camera.y < BACKGROUND_X) camera.y = BACKGROUND_X;

	// �|���S���̑S�X�V
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->update();
	}
}

// �`�揈��
void Stage::draw()
{
	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;

	// ��������`�揈��

	// �\�[�g
	polygon_vec drawing_polygons;
	for (const auto& pol_vec : polygons)
	{
		for (const auto& polygon : pol_vec.second)
		{
			drawing_polygons.push_back(polygon);
		}
	}

	// �傫�����̂��O�ɗ���悤��
	sort(drawing_polygons.begin(), drawing_polygons.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });
	
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& _polygons : polygons)
		{
			for (const auto& drawing_polygon : drawing_polygons) drawing_polygon->draw();
		}
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}

// polygons�̎w�肵�����x���Ƀ|���S����ǉ�
template<typename _T>
_T Stage::push_polygon_back(PolygonTypes type, _T polygon)
{
	polygons[type].push_back(polygon);

	return polygon;
}
