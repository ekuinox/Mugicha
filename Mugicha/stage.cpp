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

	init();
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
void Stage::exec()
{
	update();
	draw();
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

	std::map<std::string, polygon_vec>().swap(polygons); // ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���

	// �w�i�̓o�^
	polygons["BACKGROUNDS"].push_back(new Background(textures["BACKGROUND"], &camera));
	polygons["BACKGROUNDS"][0]->enable();
	polygons["BACKGROUNDS"][0]->show();

	// �v���C���̓o�^
	polygons["PLAYERS"].push_back(new Player(textures["PLAYER"], &camera));
	polygons["PLAYERS"][0]->enable();
	polygons["PLAYERS"][0]->show();

	// �g�k�ł���I�u�W�F�N�g��o�^
	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(0, 0, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][0]->enable();
	polygons["SCALABLE_OBJECTS"][0]->show();

	
	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(0, 60, 50, 50, textures["BLOCK"], 2, &camera));
	polygons["SCALABLE_OBJECTS"][1]->enable();
	polygons["SCALABLE_OBJECTS"][1]->show();
	
	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(60, 0, 50, 50, textures["BLOCK"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][2]->enable();
	polygons["SCALABLE_OBJECTS"][2]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(60, 60, 50, 50, textures["BLOCK"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][3]->enable();
	polygons["SCALABLE_OBJECTS"][3]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(120, 0, 50, 50, textures["BLOCK"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][4]->enable();
	polygons["SCALABLE_OBJECTS"][4]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(120, 60, 50, 50, textures["BLOCK"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][5]->enable();
	polygons["SCALABLE_OBJECTS"][5]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(-60, 0, 50, 50, textures["BLOCK"], 1, &camera));
	polygons["SCALABLE_OBJECTS"][6]->enable();
	polygons["SCALABLE_OBJECTS"][6]->show();


	
	// �����̃u���b�N
	polygons["BLOCKS"].push_back(new PlainSquarePolygon(100, 200, 100, 100, textures["BLOCK"], 1, &camera));
	polygons["BLOCKS"][0]->enable();
	polygons["BLOCKS"][0]->show();

	polygons["BLOCKS"].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons["BLOCKS"][1]->enable();
	polygons["BLOCKS"][1]->show();

	polygons["BLOCKS"].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons["BLOCKS"][2]->enable();
	polygons["BLOCKS"][2]->show();

	polygons["BLOCKS"].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons["BLOCKS"][3]->enable();
	polygons["BLOCKS"][3]->show();

	zoom_level = { 1, 1 };
}

// �X�V����
void Stage::update()
{
	auto current = timeGetTime();

	// �g�k
	if (GetKeyboardTrigger(DIK_O)) // �g��
	{
		zoom_level.w *= 2;
		zoom_level.h *= 2;
	}
	else if (GetKeyboardTrigger(DIK_L)) // �k��
	{
		zoom_level.w /= 2;
		zoom_level.h /= 2;
	}

	if (current - latest_update < 1) return;
	latest_update = current;

	// �S�Y�[���ύX
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->zoom(zoom_level);
	}

	// ��������X�V����
	camera.x = polygons["PLAYERS"][0]->get_coords().x;
	camera.y = polygons["PLAYERS"][0]->get_coords().y + 300;
	
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