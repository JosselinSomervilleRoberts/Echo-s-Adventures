#include "sprite.hpp"
using namespace vcl;


Sprite::Sprite() : AffichableUpdatable() {}

Sprite::Sprite(TextureLoader* textureLoader_, std::string name, int ny_, int nx_, float dt_) : AffichableUpdatable(textureLoader_) {
	ny = ny_;
	nx = nx_;
	dt_frame = dt_;
	dt_cumule = 0;

	useTexture = true;
	new_subElement();
	calcul_shape();
	textureName[0] = name + ".png";
	calcul_visual();
	visual[0].shader = 3;
}

void Sprite::calcul_shape() {
	mesh quadrangle;
	quadrangle.position = { {-1,0,-1}, { 1,0,-1}, { 1, 0,1}, {-1, 0,1} };
	quadrangle.connectivity = { {0,1,2}, {0,2,3} };
	float f = 0;
	quadrangle.uv = { {f,0.33}, {f + 0.25,0.33}, {f + 0.25,0.66}, {f,0.66} };
	quadrangle.fill_empty_field();
	shape[0] = quadrangle;
}


float Sprite::calcul_update(float dt) {
	dt_cumule += dt;
	if (dt_cumule < dt_frame) return 1;

	dt_cumule = 0;
	if (x > (nx - 1) / (float)(nx)) {
		x = 0;
		y = y - (1 / (float)(ny));
		if (y < (1 / (float)(ny))) y = 1;
	}
	shape[0].uv = { {x + 0.001,y + 0.001 - (1 / (float)(ny))}, {x - 0.001 + (1 / (float)(nx)),y + 0.001 - (1 / (float)(ny))}, {x - 0.001 + (1 / (float)(nx)),y - 0.001}, {x + 0.001,y - 0.001} }; // Associate Texture-Coordinates to the vertices of the quadrangle
	x = x + (1 / (float)(nx));
	visual[0].update_uv(shape[0].uv);

	return 1;
}

void Sprite::afficher(scene_environment& scene) {
	draw(visual[0], scene);
}

void Sprite::reset() {
	x = 0;
	y = 0;
	dt_cumule = 0;
	shape[0].uv = { {x + 0.001,y + 0.001 - (1 / (float)(ny))}, {x - 0.001 + (1 / (float)(nx)),y + 0.001 - (1 / (float)(ny))}, {x - 0.001 + (1 / (float)(nx)),y - 0.001}, {x + 0.001,y - 0.001} }; // Associate Texture-Coordinates to the vertices of the quadrangle
	visual[0].update_uv(shape[0].uv);
}