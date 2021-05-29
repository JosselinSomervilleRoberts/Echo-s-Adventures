#include "loup.hpp"

using namespace vcl;

Loup::Loup() : AffichableUpdatable() {
	t = 0;
	u = { 0,0.25,0.5,0.75 };
	angleDirection = 0;
	vitesse = 1;
	avancer = false;
	periode = 0.8f;
	gauche = false;
	droite = false;
	scaleBase = 0.1;
	scale = 0.18;
	calcul_shape();
}


void Loup::calcul_shape() {
	GLuint const shader_mesh = 3;// opengl_create_shader_program(read_text_file("shader/loup.vert.glsl"), read_text_file("shader/loup.frag.glsl"));
	std::cout << shader_mesh << std::endl;

	std::string nameFile = "assets/loup/Wolf.png";
	GLuint texture_loup = vcl::opengl_texture_to_gpu(vcl::image_load_png(nameFile), GL_MIRRORED_REPEAT  /**GL_TEXTURE_WRAP_S*/, GL_MIRRORED_REPEAT  /**GL_TEXTURE_WRAP_T*/);

	mesh_drawable corps = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/corps.obj"), shader_mesh);
	//corps.transform.scale = 0.01;
	hierarchy.add(corps, "corps");
	float h = 3.95f;

	std::string position[] = { "d", "g" };
	for (int i = 0; i < 2; i++) {
		mesh_drawable pard1 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_ar_" + position[i] + "_1.obj"), shader_mesh);
		mesh_drawable pard2 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_ar_" + position[i] + "_2.obj"), shader_mesh);
		mesh_drawable pard3 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_ar_" + position[i] + "_3.obj"), shader_mesh);
		hierarchy.add(pard1, "patte_ar_" + position[i] + "_1", "corps", -scale * vcl::vec3(0.75 - 1.5 * i, -1.4, h - 4.0f));
		hierarchy.add(pard2, "patte_ar_" + position[i] + "_2", "patte_ar_" + position[i] + "_1", -scale * vcl::vec3(0.75, -1.5, h - 2.5) + scale * vcl::vec3(0.75, -1.4, h - 4.0f));
		hierarchy.add(pard3, "patte_ar_" + position[i] + "_3", "patte_ar_" + position[i] + "_2", -scale * vcl::vec3(0.75, -2.1, h - 1.5) + scale * vcl::vec3(0.75, -1.5, h - 2.5));

		mesh_drawable pavd1 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_av_" + position[i] + "_1.obj"), shader_mesh);
		mesh_drawable pavd2 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_av_" + position[i] + "_2.obj"), shader_mesh);
		mesh_drawable pavd3 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/patte_av_" + position[i] + "_3.obj"), shader_mesh);
		hierarchy.add(pavd1, "patte_av_" + position[i] + "_1", "corps", -scale * vcl::vec3(0.75 - 1.5 * i, 1.35, h - 3.9));
		hierarchy.add(pavd2, "patte_av_" + position[i] + "_2", "patte_av_" + position[i] + "_1", -scale * vcl::vec3(0.75, 1.14, h - 2.6) + scale * vcl::vec3(0.75, 1.35, h - 3.9));
		hierarchy.add(pavd3, "patte_av_" + position[i] + "_3", "patte_av_" + position[i] + "_2", -scale * vcl::vec3(0.75, 1.18, h - 1.3) + scale * vcl::vec3(0.75, 1.14, h - 2.6));
	}


	mesh_drawable queue1 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/queue_1.obj"), shader_mesh);
	mesh_drawable queue2 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/queue_2.obj"), shader_mesh);
	mesh_drawable queue3 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/queue_3.obj"), shader_mesh);
	mesh_drawable queue4 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/queue_4.obj"), shader_mesh);
	mesh_drawable queue5 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/queue_5.obj"), shader_mesh);
	hierarchy.add(queue1, "queue_1", "corps", -scale * vcl::vec3(0, -1.7, h - 4.2));
	hierarchy.add(queue2, "queue_2", "queue_1", -scale * vcl::vec3(0, 2.2 - 3.73, 0));
	hierarchy.add(queue3, "queue_3", "queue_2", -scale * vcl::vec3(0, 3.73 - 4.8, 0));
	hierarchy.add(queue4, "queue_4", "queue_3", -scale * vcl::vec3(0, 4.8 - 5.8, 0));
	hierarchy.add(queue5, "queue_5", "queue_4", -scale * vcl::vec3(0, 5.4 - 6.2, 0));


	mesh_drawable cou1 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/cou_1.obj"), shader_mesh);
	mesh_drawable cou2 = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/cou_2.obj"), shader_mesh);
	mesh_drawable tete = mesh_drawable(vcl::mesh_load_file_obj("assets/loup/tete.obj"), shader_mesh);
	hierarchy.add(cou1, "cou_1", "corps", -scale * vcl::vec3(0, 2.5, h - 4.8));
	hierarchy.add(cou2, "cou_2", "cou_1", -scale * vcl::vec3(0, 3.1 - 2.5, 4.8 - 5.0));
	hierarchy.add(tete, "tete", "cou_2", -scale * vcl::vec3(0, 2.8 - 3.1, 5.0 - 4.9));

	for (int i = 0; i < hierarchy.elements.size(); i++) {
		hierarchy.elements.at(i).element.texture = texture_loup;
		hierarchy.elements.at(i).element.transform.scale = 0.1 * scale;
	}

	key_angles.push_back({ 0, -0.85, -1.3, -0.7, 0 });
	key_angles.push_back({ 0, 1.1, 1.3, 0.4, 0 });
	key_angles.push_back({ 0, 0.8, 1.9, 0.6, 0 });

	key_angles.push_back({ 0, -0.6, -1, -0.6, 0 });
	key_angles.push_back({ 0, 1, 1, 0.2, 0 });
	key_angles.push_back({ 0, 0, 0.4, 0.2, 0 });

	key_angles.push_back({ 0, -0.85, -1.3, -0.7, 0 });
	key_angles.push_back({ 0, 1.1, 1.3, 0.4, 0 });
	key_angles.push_back({ 0, 0.8, 1.9, 0.6, 0 });

	key_angles.push_back({ 0, -0.6, -1, -0.6, 0 });
	key_angles.push_back({ 0, 1, 1, 0.2, 0 });
	key_angles.push_back({ 0, 0, 0.4, 0.2, 0 });


	key_times = { 0,0.2,0.4,0.6, 1 };

	hierarchy["corps"].transform.translate = vec3(0, 0, 10);
}



float Loup::calcul_update(float dt) {
	return calcul_update(dt, true);
}

float Loup::calcul_update(float dt, bool peutAvancer) {
	// Compute the interpolated position
	t += dt;

	periode = 0.8f / vitesse;
	std::string name[] = { "av_d", "ar_g", "av_g", "ar_d" };
	int nb_mouvements = 0;

	for (int i = 0; i < 4; i++) {
		if (!(avancer) && (u[i] < 0.01)) {
			u[i] = 0;
		}
		else {
			nb_mouvements++;
			float prev_u = u[i];
			if (u[i] < 0.6)
				u[i] += (2 - avancer) * dt * (1 - 0.5 * (cos(vcl::pi * u[i] * 10))) / periode;
			else
				u[i] += (2 - avancer) * dt / periode;

			while (u[i] > 1) u[i] -= 1;
			if (!(avancer) && (prev_u > 0.75) && (u[i] < 0.25)) u[i] = 0;
			if (u[i] < 0) u[i] = 0;
			//if (!(avancer) && (i==0) && (prev_u > 0.9) && (u[i] >= 0)) u[0] = 0;
			//std::cout << u << std::endl;
		}
		hierarchy["patte_" + name[i] + "_1"].transform.rotate = rotation(vec3(1, 0, 0), interpolationPosTime(u[i], key_angles[3 * i + 0], key_times)[0]);
		hierarchy["patte_" + name[i] + "_2"].transform.rotate = rotation(vec3(1, 0, 0), interpolationPosTime(u[i], key_angles[3 * i + 1], key_times)[0]);
		hierarchy["patte_" + name[i] + "_3"].transform.rotate = rotation(vec3(1, 0, 0), interpolationPosTime(u[i], key_angles[3 * i + 2], key_times)[0]);
	}

	if (gauche)
		angleDirection -= dt * (3 - 1.5 * (nb_mouvements > 0)) * 180 / vcl::pi;
	else if (droite)
		angleDirection += dt * (3 - 1.5 * (nb_mouvements > 0)) * 180 / vcl::pi;


	float v = u[0];
	while (v > 0.25) v -= 0.25;
	if(peutAvancer) hierarchy["corps"].transform.translate += -vitesse * nb_mouvements * (1 + 2.5 * avancer) * 60 * scale * dt * (0.005 + 0.03 * (0.25 - v) / 0.25) * vec3(std::sin(angleDirection * vcl::pi / 180.0f), std::cos(angleDirection * vcl::pi / 180.0f), 0);
	//hierarchy["corps"].transform.rotate = rotation(vcl::vec3(0, 0, 1), -angleDirection * vcl::pi / 180.0f);

	hierarchy["cou_1"].transform.rotate = rotation(vec3(0, 0, 1), 0.1 * std::cos(t));
	hierarchy["cou_2"].transform.rotate = rotation(vec3(0, 0, 1), 0.1 * std::cos(t));
	hierarchy["tete"].transform.rotate = rotation(vec3(0, 0, 1), 0.1 * std::cos(t));

	for (int i = 1; i <= 5; i++) {
		hierarchy["queue_" + std::to_string(i)].transform.rotate = rotation(vec3(1, 0, 0), -0.1 + 0.1 * std::cos(t));
	}
	//hierarchy.update_local_to_global_coordinates();
	return 1;
}


void Loup::afficher(scene_environment& scene) {
	draw(hierarchy, scene);
}