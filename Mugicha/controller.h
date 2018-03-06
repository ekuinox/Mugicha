#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"
#include "zooming_z.h"
#include "selector.h"
#include "helpers.h"
#include "stage_thumbnail.h"
#include "always_displayed_polygon.h"

#define CAMERA_MOVE_SPEED (2.0f)

// �Q�[���Ǘ��p�̃N���X
class Controller
{
public:
	// �V�[���Ǘ��p�̗񋓌^
	enum class Scene
	{
		Ready,
		Title,
		AnimetionTitleToSelect,
		Select,
		Gaming,
		GameOver,
		GameClear,
		End,
	};
private:
	Scene scene;
	Stage::GameInfo game_info;
	time_point latest_draw;
	time_point latest_update;
	std::map<const char*, LPDIRECT3DTEXTURE9> textures;
	PolygonsVector polygons;
	AlwaysDisplayedPolygon *background; // �w�i�|���S��
	Selector *selector; // �Z���N�^�[�|���S��
	ZoomingZ *zooming_z; // Zooming��Z
	PlainSquarePolygon *zooming_ooming;
	StageThumbnail* stage_thumbnails[3];
	PlainSquarePolygon *hyousiki;
	Stage *stage;
	D3DXVECTOR2 camera;
	void update();
	void draw();
	void switch_scene(Scene _scene);
	void init();
public:
	Controller();
	void exec();
	Controller::Scene get_scene();
};