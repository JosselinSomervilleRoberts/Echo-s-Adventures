#include "firefly.hpp"


Firefly::Firefly() : AffichableUpdatable() {
	falloff = 0.1f;
	couleur = vcl::vec3(1, 0, 0);
	calcul_shape();
}


void Firefly::calcul_shape() {
	GLuint const shader_mesh = 3;

	// Parameters
	int N = 20;
	float r = 10;
	body = vcl::mesh_drawable(vcl::mesh_primitive_sphere(r, { 0,0,0 }, N, N), shader_mesh);

	orientation = vcl::vec3(1, 0, 0);
}


float Firefly::calcul_update(float dt) {
	// Compute the interpolated position
	t += dt;
	if (t > key_times[key_times.size() - 1]) t -= key_times[key_times.size() - 1];

	int nb_occurences = 3;
	intensite = 10 * (10 + 5 * cos(nb_occurences*2*vcl::pi*t/ key_times[key_times.size() - 1]));

	std::vector<vcl::vec3> result_interpol = interpolationPosTime(t, key_positions, key_times);
	vcl::vec3 const p = result_interpol[0];
	vcl::vec3 orient = result_interpol[1];
	//orientation = orient;
	float pourcent = 2.5f * dt;
	orientation = (1 - pourcent) * orientation + pourcent * orient;
	orientation = normalize(orientation);

	// Params
	float f_battement = 4;
	float montee = 0.05f;
	// Positionnement du corps
	body.transform.translate = p - vcl::vec3({ 0,0, montee * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)) });
	vcl::vec3 orientation_verticale;

	if (std::abs(orientation[1]) > std::abs(orientation[0])) {
		orientation_verticale = { 0, -orientation[2] / orientation[1], 1 };
	}
	else {
		orientation_verticale = { -orientation[2] / orientation[0], 0, 1 };
	}
	orientation_verticale = normalize(orientation_verticale);

	vcl::rotation r = rotation_between_vector({ 1,0,0 }, { 0, 0, 1 }, orientation, orientation_verticale);
	body.transform.rotate = r;
	return 1;
}


void Firefly::afficher(scene_environment& scene) {
	draw(body, scene);
}