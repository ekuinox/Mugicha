#include "stage.h"

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
			});
	}

	std::map<std::string, polygon_vec>().swap(polygons); // ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���

	// �w�i�̓o�^
	polygons["BACKGROUNDS"].push_back(new Background(textures["BACKGROUND"]));
	polygons["BACKGROUNDS"][0]->enable();
	polygons["BACKGROUNDS"][0]->show();

	// �v���C���̓o�^
	polygons["PLAYERS"].push_back(new Player(textures["PLAYER"], &camera));
	polygons["PLAYERS"][0]->enable();
	polygons["PLAYERS"][0]->show();

	// �g�k�ł���I�u�W�F�N�g��o�^
	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(210, 200, 50, 50, textures["BLOCK"], 1, 1, &camera));
	polygons["SCALABLE_OBJECTS"][0]->enable();
	polygons["SCALABLE_OBJECTS"][0]->show();

	polygons["SCALABLE_OBJECTS"].push_back(new ScalableObject(261, 200, 50, 50, textures["BLOCK"], 1, 1, &camera));
	polygons["SCALABLE_OBJECTS"][1]->enable();
	polygons["SCALABLE_OBJECTS"][1]->show();

	// �����̃u���b�N
	polygons["BLOCKS"].push_back(new PlainSquarePolygon(100, 200, 100, 100, textures["BLOCK"], 1, &camera));
	polygons["BLOCKS"][0]->enable();
	polygons["BLOCKS"][0]->show();
}

// �X�V����
void Stage::update()
{
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

	// ��������X�V����
	camera.x = polygons["PLAYERS"][0]->get_coords().x;
	camera.y = polygons["PLAYERS"][0]->get_coords().y;

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