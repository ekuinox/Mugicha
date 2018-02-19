#include "stage.h"

void Stage::init1()
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
			{ "SAMPLE1", "./resources/textures/sample1.png" },
			});
	}

	std::map<enum PolygonTypes, polygon_vec>().swap(polygons); // ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���

															   // �w�i�̓o�^
	polygons[BACKGROUND].push_back(new Background(textures["BACKGROUND"], &camera));
	polygons[BACKGROUND][0]->enable();
	polygons[BACKGROUND][0]->show();

	// �v���C���̓o�^
	polygons[PLAYER].push_back(new Player(textures["PLAYER"], &camera, polygons));
	polygons[PLAYER].back()->enable();
	polygons[PLAYER].back()->show();

	// �g�k�ł���I�u�W�F�N�g��o�^
	polygons[SCALABLE_OBJECT].push_back(new ScalableObject(25, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons[SCALABLE_OBJECT].back()->enable();
	polygons[SCALABLE_OBJECT].back()->show();

	polygons[SCALABLE_OBJECT].push_back(new ScalableObject(100, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
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