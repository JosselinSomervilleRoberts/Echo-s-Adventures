#include "loup.hpp"


Loup::Loup() : AffichableUpdatable() {
	calcul_shape();
}


void Loup::calcul_shape() {
	// Parameters
	int N = 20;

	float r_eyes = 0.085f;
	float r_head = 0.45f;
	float angle_oreille = vcl::pi / 4.0f;
	float angle_oreille2 = vcl::pi / 4.0f;
	float h_oreille = r_head / 2.0f;

	float x_body = 1.6f;
	float y_body = 0.8f;
	float z_body = 0.6f;

	float l1_leg = 0.6f;
	float r1_leg = 0.2f;
	float l2_leg = 0.6f;
	float r2_leg = 0.2f;
	float r_articulation = 0.25f;
	float r_articulation2 = 0.15f;

	float ecartement_pattes_x = 0.7 * x_body;
	float ecartement_pattes_y = 0.75 * y_body;


	// Geometry of the eyes: black spheres
	vcl::vec3 couleur = vcl::vec3(0.6f, 0.4f, 0.28f);
	vcl::mesh eye_m = vcl::mesh_primitive_sphere(r_eyes, { 0,0,0 }, N, N);
	eye_m.color.fill({ 0.1, 0.1, 0.1 });
	vcl::mesh_drawable eye = vcl::mesh_drawable(eye_m);
	vcl::mesh body_m = vcl::mesh_primitive_ellipsoid(vcl::vec3{ x_body, y_body, z_body }, { 0,0,0 }, 4 * N, N);
	body_m.color.fill(couleur);
	vcl::mesh_drawable body = vcl::mesh_drawable(body_m);

	vcl::mesh phal1_m = vcl::mesh_primitive_ellipsoid(vcl::vec3{ l1_leg, r1_leg, r1_leg }, { 0,0,0 }, N, (int)(N / 2.0f));
	phal1_m.color.fill(couleur);
	vcl::mesh_drawable phal1 = vcl::mesh_drawable(phal1_m);
	vcl::mesh phal2_m = vcl::mesh_primitive_ellipsoid(vcl::vec3{ l2_leg, r2_leg, r2_leg }, { 0,0,0 }, N, (int)(N / 2.0f));
	phal2_m.color.fill(couleur);
	vcl::mesh_drawable phal2 = vcl::mesh_drawable(phal2_m);
	vcl::mesh articulation_m = vcl::mesh_primitive_sphere(r_articulation, { 0,0,0 }, 2 * N, N);
	articulation_m.color.fill(couleur);
	vcl::mesh_drawable articulation = vcl::mesh_drawable(articulation_m);
	vcl::mesh articulation2_m = vcl::mesh_primitive_sphere(r_articulation2, { 0,0,0 }, 2 * N, N);
	articulation2_m.color.fill(couleur);
	vcl::mesh_drawable articulation2 = vcl::mesh_drawable(articulation2_m);


	vcl::mesh head_m = vcl::mesh_primitive_sphere(r_head, { 0,0,0 }, 2 * N, N);
	head_m.color.fill(couleur);
	vcl::mesh_drawable head = vcl::mesh_drawable(head_m);
	vcl::mesh museau_m = vcl::mesh_primitive_ellipsoid(vcl::vec3{ 1.3f*r_head, r_head/1.4f, r_head/1.7f }, { 0,0,0 }, N, (int)(N / 2.0f));
	museau_m.color.fill(couleur);
	vcl::mesh_drawable museau = vcl::mesh_drawable(museau_m);
	r_head = r_head / 2.0f;
	vcl::vec3 p1 = vcl::vec3(0.0f, r_head * sin(angle_oreille), r_head * cos(angle_oreille));
	vcl::mesh oreille1_m = vcl::mesh_primitive_triangle(p1, vcl::vec3(0.0f, r_head * sin(angle_oreille+angle_oreille2), r_head * cos(angle_oreille+angle_oreille2)), vcl::vec3(0.0f, (r_head+h_oreille) * sin(angle_oreille+angle_oreille2/2.0f), (r_head + h_oreille) * cos(angle_oreille + angle_oreille2 / 2.0f)));
	oreille1_m.color.fill(couleur);
	vcl::mesh_drawable oreille1 = vcl::mesh_drawable(oreille1_m);

	hierarchy.add(body, "body");
	hierarchy.add(head, "head", "body", vcl::vec3(x_body + 0.4f*r_head, 0, 0));
	hierarchy.add(museau, "museau", "head", vcl::vec3(r_head*0.5f, 0, -r_head*0.2f));
	hierarchy.add(eye, "eye_left", "head", r_head * vcl::vec3(0.63f, -1 / 3.0f, 1 / 2.1f));
	hierarchy.add(eye, "eye_right", "head", r_head * vcl::vec3(0.63f, 1 / 3.0f, 1 / 2.1f));
	hierarchy.add(oreille1, "oreille1", "head", r_head * vcl::vec3(0.63f, 1 / 3.0f, 1 / 2.1f));

	// Eyes positions are set with respect to some ratio of the body
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			std::string name = std::to_string(x) + "_" + std::to_string(y);
			hierarchy.add(articulation, "leg_" +  name, "body", vcl::vec3((2.0f*x - 1.0f)*ecartement_pattes_x, (2.0f*y - 1.0f)*ecartement_pattes_y, 0));
			hierarchy.add(phal1, "phal1_" + name, "leg_" + name, vcl::vec3(3*l1_leg / 4.0f, 0, 0));
			hierarchy.add(articulation2, "articulation1_" + name, "phal1_" + name, vcl::vec3(l1_leg, 0, 0));
			hierarchy.add(phal2, "phal2_" + name, "articulation1_" + name, vcl::vec3(3*l2_leg / 4.0f, 0, 0));
		}
	}
	
	/*
	hierarchy.add(aile, "aile_1_left", "body", vcl::vec3(0, 0.65 * y_body, 0.3 * z_body));
	hierarchy.add(aile2, "aile_2_left", "aile_1_left", vcl::vec3(0, p_aile1, 0));
	hierarchy.add(aile, "aile_1_right", "body", vcl::vec3(0, -0.65 * y_body, 0.3 * z_body));
	hierarchy.add(aile2, "aile_2_right", "aile_1_right", vcl::vec3(0, p_aile1, 0));

	hierarchy.add(head, "head", "body", vcl::vec3(0.85 * x_body + r_head, 0., 0.4 * z_body));
	hierarchy.add(eye, "eye_left", "head", r_head * vcl::vec3(0.55, -1 / 3.0f, 1 / 2.0f));
	hierarchy.add(eye, "eye_right", "head", r_head * vcl::vec3(0.55, 1 / 3.0f, 1 / 2.0f));

	// Set the left part of the body arm: shoulder-elbow-arm
	hierarchy.add(bec, "bec", "head", vcl::vec3(sqrt(r_head * r_head - r_bec * r_bec) - 0.001, 0, 0));
	*/

	orientation = vcl::vec3(1, 0, 0);
}

float mouv(float t) {
	t -= int(t);
	if (t < 0.5)
		return sin(2 * vcl::pi * t);
	else
		return 0;
}


float Loup::calcul_update(float dt) {
	// Compute the interpolated position
	t += dt;
	//if (t > key_times[key_times.size() - 1]) t -= key_times[key_times.size() - 1];

	std::vector<vcl::vec3> result_interpol = { vcl::vec3(0, 0, 20), vcl::vec3(0,0,0) };// interpolationPosTime(t, key_positions, key_times);
	vcl::vec3 const p = result_interpol[0];
	vcl::vec3 orient = result_interpol[1];
	//orientation = orient;
	float pourcent = 2.5f * dt;
	orientation = (1 - pourcent) * orientation + pourcent * orient;
	orientation = normalize(orientation);

	// Params
	float r_mouv_tete = 0.2f;
	float r_mouv_aile_1 = 0.5f;
	float r_mouv_aile_2 = 0.8f;

	float f_battement = 4;
	float montee = 0.05f;
	// Positionnement du corps
	hierarchy["body"].transform.translate = p - vcl::vec3({ 0,0, 0});
	hierarchy["body"].transform.scale = 0.5;

	/*
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
	hierarchy["aile_2_right"].transform.rotate = vcl::rotation({ 1,0,0 }, -r_mouv_aile_2 * std::sin(f_battement * 2 * 3.14f * (t - 0.4f)));*/

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			std::string name = std::to_string(x) + "_" + std::to_string(y);
			float decalage = 0;
			if (x == 1 && y == 1) decalage = 0.25;
			if (x == 0 && y == 1) decalage = 0.5;
			if (x == 1 && y == 0) decalage = 0.75;
			hierarchy["leg_" + name].transform.rotate = vcl::rotation(vcl::vec3(0, 1, 0), (4*vcl::pi/8.0f)  + (vcl::pi/3.0f) * mouv(0.3*t+ decalage));
			hierarchy["articulation1_" + name].transform.rotate = vcl::rotation(vcl::vec3(0, 1, 0), - vcl::pi / 4.0f - (vcl::pi / 3.0f) * mouv(0.3*t + decalage));
		}
	}

	hierarchy.update_local_to_global_coordinates();
	return 1;
}


void Loup::afficher(scene_environment& scene) {
	draw(hierarchy, scene);
}