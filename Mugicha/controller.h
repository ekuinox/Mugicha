#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"
#include "selector.h"

// �V�[���Ǘ��p�̗񋓌^
enum scene
{
	Ready,
	Title,
	Select,
	Gaming,
	GameOver,
	GameClear,
	End,
};

// �Q�[���Ǘ��p�̃N���X
class Controller
{
private:
	enum scene scene;
	int loops;
	std::chrono::system_clock::time_point latest_draw;
	std::chrono::system_clock::time_point latest_update;
	std::map<const char*, LPDIRECT3DTEXTURE9> textures;
	polygon_vec polygons;
	Background *background; // �w�i�|���S��
	Selector *selector; // �Z���N�^�[�|���S��
	Stage *stage;
	D3DXVECTOR2 camera;
	void update();
	void draw();
public:
	Controller();
	~Controller();
	void init();
	void exec();
	void switch_scene(enum scene _scene);
};