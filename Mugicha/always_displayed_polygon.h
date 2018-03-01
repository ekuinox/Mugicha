#pragma once
#include "plain_square_polygon.h"

/*
* AlwaysDisplayedPolygon
* Cameraに影響されないポリゴンを作る
*
*/

class AlwaysDisplayedPolygon : public PlainSquarePolygon
{
protected:
	void generate_vertexes();
public:
};

