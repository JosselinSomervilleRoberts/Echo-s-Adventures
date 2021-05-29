#include "fire.hpp"

Fire::Fire() : AffichableUpdatable() {
	N = 16;
}

Fire::Fire(TextureLoader* textureLoader_, int N_) : AffichableUpdatable(textureLoader_) {
	N = N_;
	calcul_shape();
}

Fire::~Fire() {
	delete sprite;
}

void Fire::calcul_shape() {
	sprite = new Sprite(textureLoader, "textures/fire", 8, 8, 0.03);
}

float Fire::calcul_update(float dt) {
	return sprite->calcul_update(dt);
}

void Fire::afficher(scene_environment& scene) {
	for (int i = 0; i < 16; i++) {
		sprite->set_rotation(vcl::rotation(vcl::vec3(0, 0, 1), vcl::pi * i * 2 / ((float)(N))));
		sprite->afficher(scene);
	}
}

void Fire::set_pos(vcl::vec3 pos_) {
	sprite->set_pos(pos_);
}