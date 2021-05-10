#include "oiseau.hpp"


Oiseau::Oiseau() : AffichableUpdatable() {
	calcul_shape();
}


void Oiseau::calcul_shape() {
	// Parameters
	int N = 20;

	float r_eyes = 0.085f;
	float r_head = 0.34f;
	float r_bec = 0.14f;
	float l_bec = 0.26f;

	float x_body = 0.8f;
	float y_body = 0.6f;
	float z_body = 0.5f;

	float l_aile1 = 1.1f;
	float l_aile2 = 0.8f;
	float l_aile3 = 0.3f;
	float p_aile1 = 0.7f;
	float p_aile2 = 0.5f;

	// Geometry of the eyes: black spheres
	vcl::mesh eye_m = vcl::mesh_primitive_sphere(r_eyes, { 0,0,0 }, N, N);
	eye_m.color.fill({ 0.1, 0.1, 0.1 });
	vcl::mesh_drawable eye = vcl::mesh_drawable(eye_m);
	vcl::mesh_drawable head = vcl::mesh_drawable(vcl::mesh_primitive_sphere(r_head, { 0,0,0 }, 2 * N, N));
	vcl::mesh_drawable body = vcl::mesh_drawable(vcl::mesh_primitive_ellipsoid(vcl::vec3{ x_body, y_body, z_body }, { 0,0,0 }, 4 * N, N));
	vcl::mesh bec_m = vcl::mesh_primitive_cone(r_bec, l_bec, { 0,0,0 }, { 1 ,0,0 }, true, 2 * N, N);
	bec_m.color.fill({ 1.0, 0.5, 0.15 });
	vcl::mesh_drawable bec = vcl::mesh_drawable(bec_m);

	vcl::mesh aile_1 = vcl::mesh_primitive_quadrangle({ -l_aile1 / 2.0, 0, 0 }, { +l_aile1 / 2.0, 0, 0 }, { +l_aile2 / 2.0, p_aile1, 0 }, { -l_aile2 / 2.0, p_aile1, 0 });
	vcl::mesh_drawable aile = vcl::mesh_drawable(aile_1);

	vcl::mesh aile_2 = vcl::mesh_primitive_quadrangle({ -l_aile2 / 2.0, 0, 0 }, { +l_aile2 / 2.0, 0, 0 }, { +l_aile3 / 2.0, p_aile2, 0 }, { -l_aile3 / 2.0, p_aile2, 0 });
	vcl::mesh_drawable aile2 = vcl::mesh_drawable(aile_2);

	hierarchy.add(body, "body");

	// Eyes positions are set with respect to some ratio of the body
	hierarchy.add(head, "head", "body", vcl::vec3(0.85 * x_body + r_head, 0., 0.4 * z_body));
	hierarchy.add(aile, "aile_1_left", "body", vcl::vec3(0, 0.65 * y_body, 0.3 * z_body));
	hierarchy.add(aile2, "aile_2_left", "aile_1_left", vcl::vec3(0, p_aile1, 0));
	hierarchy.add(aile, "aile_1_right", "body", vcl::vec3(0, -0.65 * y_body, 0.3 * z_body));
	hierarchy.add(aile2, "aile_2_right", "aile_1_right", vcl::vec3(0, p_aile1, 0));

	hierarchy.add(eye, "eye_left", "head", r_head * vcl::vec3(0.55, -1 / 3.0f, 1 / 2.0f));
	hierarchy.add(eye, "eye_right", "head", r_head * vcl::vec3(0.55, 1 / 3.0f, 1 / 2.0f));

	// Set the left part of the body arm: shoulder-elbow-arm
	hierarchy.add(bec, "bec", "head", vcl::vec3(sqrt(r_head * r_head - r_bec * r_bec) - 0.001, 0, 0));


	orientation = vcl::vec3(1, 0, 0);
}


float Oiseau::calcul_update(float dt) {
	// Compute the interpolated position
	t += dt;
	if (t > key_times[key_times.size() - 1]) t -= key_times[key_times.size() - 1];

	std::vector<vcl::vec3> result_interpol = interpolationPosTime(t, key_positions, key_times);
	vcl::vec3 const p = result_interpol[0];
	vcl::vec3 orient = result_interpol[1];
	//orientation = orient;
	float pourcent = 2.5f*dt;
	orientation = (1 - pourcent) * orientation + pourcent * orient;
	orientation = normalize(orientation);

	// Params
	float r_mouv_tete = 0.2f;
	float r_mouv_aile_1 = 0.5f;
	float r_mouv_aile_2 = 0.8f;

	float f_battement = 4;
	float montee = 0.05f;

	// Positionnement du corps
	hierarchy["body"].transform.translate = p - vcl::vec3({ 0,0, montee * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)) });
	hierarchy["body"].transform.scale = 0.5;
	vcl::vec3 orientation_verticale;

	if (std::abs(orientation[1]) > std::abs(orientation[0])) {
		orientation_verticale = { 0, -orientation[2] / orientation[1], 1 };
	}
	else {
		orientation_verticale = { -orientation[2] / orientation[0], 0, 1 };
	}
	orientation_verticale = normalize(orientation_verticale);

	vcl::rotation r = rotation_between_vector({ 1,0,0 }, { 0, 0, 1 }, orientation, orientation_verticale);
	hierarchy["body"].transform.rotate = r;

	// Mouvement des autres éléments
	hierarchy["head"].transform.rotate = vcl::rotation({ 0,1,0 }, r_mouv_tete * std::sin(2 * 3.14f * (t - 0.4f)));
	hierarchy["aile_1_left"].transform.rotate = vcl::rotation({ 1,0,0 }, r_mouv_aile_1 * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)));
	hierarchy["aile_2_left"].transform.rotate = vcl::rotation({ 1,0,0 }, r_mouv_aile_2 * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)));
	hierarchy["aile_1_right"].transform.rotate = vcl::rotation({ 1,0,0 }, 3.14f - r_mouv_aile_1 * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)));
	hierarchy["aile_2_right"].transform.rotate = vcl::rotation({ 1,0,0 }, -r_mouv_aile_2 * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)));
	hierarchy.update_local_to_global_coordinates();
	return 1;
}


void Oiseau::afficher(scene_environment& scene) {
	draw(hierarchy, scene);
}