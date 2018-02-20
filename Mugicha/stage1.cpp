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

	std::map<enum PolygonTypes, polygon_vec>().swap(polygons); // Ç∑Ç◊Çƒñ≥Ç…Ç∑ÇÈÅ@å„ÇÃÇ±Ç∆ÇÕçlÇ¶ÇƒÇ»Ç¢Å@Ç∑Ç›Ç‹ÇπÇÒ

	// îwåiÇÃìoò^
	background = push_polygon_back(BACKGROUND, new Background(textures["BACKGROUND"], &camera));
	background->enable();
	background->show();

	// ÉvÉåÉCÉÑÇÃìoò^
	player = push_polygon_back(PLAYER, new Player(textures["PLAYER"], &camera, polygons));
	player->enable();
	player->show();

	// ägèkÇ≈Ç´ÇÈÉIÉuÉWÉFÉNÉgÇìoò^
	for (auto i = 0; i < 3; ++i)
	{
		push_polygon_back(SCALABLE_OBJECT, new ScalableObject(25 + i * 55, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
		polygons[SCALABLE_OBJECT].back()->enable();
		polygons[SCALABLE_OBJECT].back()->show();
	}

	push_polygon_back(SCALABLE_OBJECT, new ScalableObject(25, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons[SCALABLE_OBJECT].back()->enable();
	polygons[SCALABLE_OBJECT].back()->show();

	push_polygon_back(SCALABLE_OBJECT, new ScalableObject(80, 25, 50, 50, textures["SAMPLE1"], 1, &camera));
	polygons[SCALABLE_OBJECT].back()->enable();
	polygons[SCALABLE_OBJECT].back()->show();

	// å¥ì_
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// xé≤ÉKÉCÉhÉâÉCÉì
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	// yé≤ÉKÉCÉhÉâÉCÉì
	polygons[PLAIN].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[PLAIN].back()->enable();
	polygons[PLAIN].back()->show();

	zoom_level = { 1, 1 };
	zoom_sign = ZERO;
#ifdef _DEBUG
		printf("LOADED STAGE1\n");
#endif
}