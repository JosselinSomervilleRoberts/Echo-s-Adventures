#pragma once
#include "vcl/vcl.hpp"
#include "affichableUpdatable.hpp"
#include "sprite.hpp"
using namespace vcl;

class Fire : public AffichableUpdatable {
private:
	int N;
	Sprite* sprite;

public:
	Fire();
	Fire(TextureLoader* textureLoader_, int N_);
	~Fire();

	void calcul_shape();
	float calcul_update(float dt);
	void afficher(scene_environment& scene);
	void set_pos(vcl::vec3 pos_);
};