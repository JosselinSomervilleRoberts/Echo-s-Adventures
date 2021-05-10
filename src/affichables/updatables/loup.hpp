#pragma once
#include "affichableUpdatable.hpp"
#include "vcl/vcl.hpp"
#include "interpolation.hpp"

#include <iostream>
#include <stdlib.h>

class Loup : public AffichableUpdatable {
private:
	vcl::hierarchy_mesh_drawable hierarchy;
	float t = 0;
	vcl::vec3 orientation;

public:
	Loup();

	vcl::buffer<vcl::vec3> key_positions;
	vcl::buffer<float> key_times;
	void calcul_shape();
	float calcul_update(float dt);
	void afficher(scene_environment& scene);
};