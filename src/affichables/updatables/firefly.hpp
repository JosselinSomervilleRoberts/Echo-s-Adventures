#pragma once
#include "affichableUpdatable.hpp"
#include "vcl/vcl.hpp"
#include "interpolation.hpp"

class Firefly : public AffichableUpdatable {
private:
	vcl::mesh_drawable body;
	float t = 0;
	vcl::vec3 orientation;


public:
	vcl::vec3 couleur;
	float intensite;
	float falloff;

	Firefly();

	vcl::buffer<vcl::vec3> key_positions;
	vcl::buffer<float> key_times;
	void calcul_shape();
	float calcul_update(float dt);
	void afficher(scene_environment& scene);
	vcl::vec3 getPos() { return body.transform.translate; };
};