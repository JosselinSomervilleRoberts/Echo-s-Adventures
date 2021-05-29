#pragma once
#include "vcl/vcl.hpp"
#include "affichableUpdatable.hpp"
using namespace vcl;

class Sprite : public AffichableUpdatable {
private:
	int ny;
	int nx;
	float x = 0;
	float y = 1;
	float dt_frame;
	float dt_cumule;

public:
	Sprite();
	Sprite(TextureLoader* textureLoader_, std::string name, int ny, int nx, float dt);

	void calcul_shape();
	float calcul_update(float dt);
	void afficher(scene_environment& scene);
	void reset();
};