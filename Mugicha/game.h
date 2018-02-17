#pragma once
#include <map>
#include <vector>
#include <string>
#include "stage.h"

// �V�[���Ǘ��p�̗񋓌^
enum scene
{
	TITLE,
	MAIN,
	GAMEOVER,
	GAMECLEAR,
	END,
};

// �Q�[���Ǘ��p�̃N���X
class Game
{
private:
	enum scene scene;
	int loops;
	DWORD latest_draw;
	DWORD latest_update;
	Stage *stage;
	void update();
	void draw();
public:
	Game();
	~Game();
	void init();
	void exec();
	void switch_scene(enum scene _scene);
	bool main(bool reset = false);
};