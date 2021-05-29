#pragma once
#include "affichableUpdatable.hpp"
#include "vcl/vcl.hpp"
#include "interpolation.hpp"

#include <iostream>
#include <stdlib.h>

using namespace vcl;

class Loup : public AffichableUpdatable {
private:
	float t;
	vcl::vec3 orientation;
	std::vector<vcl::buffer<float>> key_angles;
	vcl::buffer<float> key_times;
	
	float periode;

public:
	bool avancer;
	bool gauche;
	bool droite;
	float vitesse;
	float angleDirection;
	std::vector<float> u;
	vcl::hierarchy_mesh_drawable hierarchy;
	Loup();

	void calcul_shape();
	float calcul_update(float dt);
	float calcul_update(float dt, bool peutAvancer);
	void afficher(scene_environment& scene);
};