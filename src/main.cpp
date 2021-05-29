#include "vcl/vcl.hpp"
#include "affichables/updatables/corde.hpp"
#include "display/scene_environment.hpp"
#include "utils/utils.hpp"
#include "affichables/basic/cylindre.hpp"
#include "affichables/updatables/pont.hpp"
#include "utils/textureLoader.hpp"
#include "affichables/models/objModel.hpp"
#include "affichables/terrain/terrain.hpp"
#include "affichables/terrain/chunk.hpp"
#include "affichables/updatables/oiseau.hpp"
#include "affichables/updatables/loup.hpp"
#include "display/environment_map.hpp"
#include "display/plane_renderer.hpp"
#include "affichables/updatables/fire.hpp"
#include "affichables/updatables/sprite.hpp"
#include <iostream>
#include <list>
#include <stdlib.h>


using namespace vcl;

struct keyboard_state_parameters {
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool a = false;
	bool q = false;
	bool space = false;
};

struct gui_parameters {
	bool display_frame = true;
};

struct user_interaction_parameters {
	vec2 mouse_prev;
	timer_fps fps_record;
	mesh_drawable global_frame;
	gui_parameters gui;
	bool cursor_on_gui;
	keyboard_state_parameters keyboard_state;
	float speed = 2.0f;
	float height = 1.0f;
};
user_interaction_parameters user;


scene_environment scene;
TextureLoader textureLoader("");
timer_interval timer;
int width, height;


void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void display_segment(vec3 const& a, vec3 const& b);

void initialize_data();
void display_interface();
void display_frame();



mesh_drawable sphere;
mesh_drawable disc;
mesh_drawable cube;
GLuint shader_mesh_lights;
//timer_event_periodic timer(0.6f);

segments_drawable segments;
Cylindre* cylindre;
Pont* pont;
Terrain* terr;
Oiseau* oiseau;
Loup* loup;
ObjModel* duck;

vcl::vec3 pos1;
vcl::vec3 pos2;
vcl::vec3 pos3;
vcl::vec3 pos4;
Corde corde;
double t;
std::vector<ObjModel> tree;

mesh terrain_cpu;
mesh_drawable terrain;
std::vector<vcl::vec3> tree_position;
std::vector<int> types;


Sprite* sprite;



plane_renderer rend;
plane_renderer rend2;
float rouge = 1;
float vert = 1;
float bleu = 1;
float distance_zone = 8;
float spotlight_falloff = 0.5f;


float cam_angle_vertic = 38;
float cam_angle_horizont = 0;
float R = 7;
vec3 camera_position;
vec3 camera_target_position;
bool separate = true;

float debutAnimationRune = 0;
bool animationRuneActive = false;

// Store keyboard state
// left-right / up-down key
/*
void keyboard_callback(GLFWwindow*, int key, int, int action, int)
{
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) user.keyboard_state.a = true;
		if (action == GLFW_RELEASE) user.keyboard_state.a = false;
	}

	if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) user.keyboard_state.q = true;
		if (action == GLFW_RELEASE) user.keyboard_state.q = false;
	}

	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) user.keyboard_state.up = true;
		if (action == GLFW_RELEASE) user.keyboard_state.up = false;
	}

	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) user.keyboard_state.down = true;
		if (action == GLFW_RELEASE) user.keyboard_state.down = false;
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) user.keyboard_state.left = true;
		if (action == GLFW_RELEASE) user.keyboard_state.left = false;
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) user.keyboard_state.right = true;
		if (action == GLFW_RELEASE) user.keyboard_state.right = false;
	}
}
*/


void keyboard_callback(GLFWwindow*, int key, int, int action, int)
{
	//std::cout << key << std::endl;
	if (key == 81) {
		if (action == GLFW_PRESS) user.keyboard_state.a = true;
		if (action == GLFW_RELEASE) user.keyboard_state.a = false;
	}

	if (key == 65) {
		if (action == GLFW_PRESS) user.keyboard_state.q = true;
		if (action == GLFW_RELEASE) user.keyboard_state.q = false;
	}

	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			if (!(user.keyboard_state.up)) loup->u = { (loup->u[0] + 0.0f), (loup->u[0] + 0.25f), (loup->u[0] + 0.2f), (loup->u[0] + 0.75f) };
			user.keyboard_state.up = true;
		}
		if (action == GLFW_RELEASE) user.keyboard_state.up = false;
	}

	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) user.keyboard_state.down = true;
		if (action == GLFW_RELEASE) user.keyboard_state.down = false;
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) user.keyboard_state.left = true;
		if (action == GLFW_RELEASE) user.keyboard_state.left = false;
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) user.keyboard_state.right = true;
		if (action == GLFW_RELEASE) user.keyboard_state.right = false;
	}
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			if (!(user.keyboard_state.space) && (user.keyboard_state.up)) {
				loup->u = { (loup->u[0] + 0.0f), (loup->u[0] + 0.5f), (loup->u[0] + 0.0f), (loup->u[0] + 0.5f) };
				loup->vitesse = 2;
			}
			user.keyboard_state.space = true;
		}
		if (action == GLFW_RELEASE) {
			user.keyboard_state.space = false;
			if (user.keyboard_state.up) loup->u = { (loup->u[0] + 0.0f), (loup->u[0] + 0.25f), (loup->u[0] + 0.2f), (loup->u[0] + 0.75f) };
			loup->vitesse = 1;
		}
	}
}



int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	width = 1280, height = 1024;
	GLFWwindow* window = create_window(width, height);
	window_size_callback(window, width, height);
	std::cout << opengl_info_display() << std::endl;;

	imgui_init(window);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	std::cout << "Initialize data ..." << std::endl;
	initialize_data();

	std::cout << "Start animation loop ..." << std::endl;
	user.fps_record.start();
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		user.fps_record.update();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		imgui_create_frame();
		if (user.fps_record.event) {
			std::string const title = "VCL Display - " + str(user.fps_record.fps) + " fps";
			glfwSetWindowTitle(window, title.c_str());
		}

		ImGui::Begin("GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		user.cursor_on_gui = ImGui::GetIO().WantCaptureMouse;

		if (user.gui.display_frame) draw(user.global_frame, scene);

		display_interface();
		display_frame();


		ImGui::End();
		imgui_render_frame(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void initialize_data()
{
	//rend = plane_renderer();
	shader_mesh_lights = opengl_create_shader_program(read_text_file("shader/mesh_lights.vert.glsl"), read_text_file("shader/mesh_lights.frag.glsl"));
	// Read shaders
	GLuint const shader_skybox = opengl_create_shader_program(read_text_file("shader/skybox.vert.glsl"), read_text_file("shader/skybox.frag.glsl"));
	//GLuint const shader_environment_map = opengl_create_shader_program(read_text_file("shader/environment_map.vert.glsl"), read_text_file("shader/environment_map.frag.glsl"));
	std::cout << "shader_mesh_lights = " << shader_mesh_lights << std::endl;

	// Read cubemap texture
	GLuint texture_cubemap = cubemap_texture("textures/skybox/");

	/* initialize random seed: */
	srand(time(NULL));

	// Basic setups of shaders and camera
	GLuint const shader_mesh = opengl_create_shader_program(opengl_shader_preset("mesh_vertex"), opengl_shader_preset("mesh_fragment"));
	GLuint const shader_single_color = opengl_create_shader_program(opengl_shader_preset("single_color_vertex"), opengl_shader_preset("single_color_fragment"));
	mesh_drawable::default_shader = shader_mesh_lights;
	mesh_drawable::default_texture = opengl_texture_to_gpu(image_raw{ 1,1,image_color_type::rgba,{255,255,255,255} });
	segments_drawable::default_shader = shader_single_color;
	segments = segments_drawable({ {0,0,0},{1,0,0} });


	//initialize_fbo(rend);



	// Cube used to display the skybox
	cube = mesh_drawable(mesh_primitive_cube({ 0,0,0 }, 2.0f), shader_skybox, texture_cubemap);



	// ================ Création de la scene ==================== //
	user.global_frame = mesh_drawable(mesh_primitive_frame());

	// Pour la camera
	//scene.camera.distance_to_center = 10.0f;
	//scene.camera.look_at({ 3,1,2 }, { 0,0,0.5 }, { 0,0,1 });

	// Pour les ombres
	//scene.depth_map = initialize_depth_map();
	scene.projection_light = projection_orthographic(-10, 10, -10, 10, 0, 30); // orthographic projection for simplicity
	scene.light.distance_to_center = 5.0f;
	scene.light.manipulator_rotate_spherical_coordinates(pi / 4.0f, pi / 4.0f);




	// Création du terrain
	int N = 10;
	float chunk_size = 4;
	terr = new Terrain(chunk_size, N, &textureLoader);
	//scene.camera.position_camera[2] = user.height + terr->get_heightOfTerrain(scene.camera.position_camera[0], scene.camera.position_camera[1]);


	// Création du pont
	float deviation = 0.;
	pos1 = { -8.0f,3.0f, std::max(terr->get_heightOfTerrain(-8,3), terr->get_heightOfTerrain(-8,5)) };
	pos2 = { 10.0f,3.0f, std::max(terr->get_heightOfTerrain(10,3), terr->get_heightOfTerrain(10,5)) };
	pos3 = { -8.0f,5.0f, std::max(terr->get_heightOfTerrain(-8,3), terr->get_heightOfTerrain(-8,5)) };
	pos4 = { 10.0f,5.0f, std::max(terr->get_heightOfTerrain(10,3), terr->get_heightOfTerrain(10,5)) };
	t = 0;
	pont = new Pont(&pos1, &pos2, &pos3, &pos4, 40, 10, &textureLoader);


	// Création des animaux
	oiseau = new Oiseau();
	loup = new Loup();
	//loup->set_scale(0.01);

	// Key positions
	int N_steps = 20;
	int R = 18;
	for (int i = 0; i < N_steps + 1; i++) {
		oiseau->key_positions.push_back({ R * std::sin(3.14f * 2 * i / float(N_steps)), R * std::cos(3.14f * 2 * i / float(N_steps)), 5 + double(int(rand() % 30)) / 20.0 });
		oiseau->key_times.push_back(double(i * 2.2));
	}

	// Set timer bounds
	//  You should adapt these extremal values to the type of interpolation
	size_t const N2 = oiseau->key_times.size();
	timer.t_min = oiseau->key_times[0];    // Start the timer at the first time of the keyframe
	timer.t_max = oiseau->key_times[N2 - 1];  // Ends the timer at the last time of the keyframe
	timer.t = timer.t_min;

	// Création du canard
	duck = new ObjModel("house5", &textureLoader);
	float x = 0;
	float y = 0;
	duck->set_pos(vcl::vec3(x, y, 0 + terr->get_heightOfTerrain(x,y)));
	duck->set_rotation(vcl::rotation(vcl::vec3(0, 0, 1), vcl::pi / 2.0f));
	//scene.spotlight_color[1] = { 0.3f, 0.3f, 1.0f };
	//scene.spotlight_position[1] = { x,y,0.5+ terr->get_heightOfTerrain(x,y)};// loup->hierarchy["corps"].transform.translate + vcl::vec3(0, 0, 3);
	//scene.spotlight_falloff[1] = 0.1;// spotlight_falloff * 0.1f;
	//scene.spotlight_intensity[1] = 20;
	scene.ajouterLumiere({ x,y,0.5 + terr->get_heightOfTerrain(x,y) }, { 0.3f, 0.3f, 1.0f }, 0.1, 50);

	sphere = mesh_drawable(mesh_primitive_sphere(0.2f));
	sphere.transform.translate[2] = 10;



	// Shader computing an image filter in the second pass
	GLuint const shader_screen_render = opengl_create_shader_program(read_text_file("shader/screen_render.vert.glsl"), read_text_file("shader/screen_render.frag.glsl"));
	GLuint const shader_screen_render2 = opengl_create_shader_program(read_text_file("shader/screen_render2.vert.glsl"), read_text_file("shader/screen_render2.frag.glsl"));

	// The result of the second pass is displayed on a quad
	//rend.quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	//rend2.quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,-1 }, { 0,-1,-1 }, { 0,1,-1 }, { -1,1,-1 }));
	rend.initiate(shader_screen_render, width, height);
	rend2.initiate(shader_screen_render2, width, height);


	//scene.camera.position_camera = vcl::vec3(0, 0, 0);
	//scene.camera.manipulator_rotate_roll_pitch_yaw(0, vcl::pi / 2.0f, 0);

	sprite = new Sprite(&textureLoader, "textures/rune_verte", 10, 6, 0.05);// new Fire(&textureLoader, 16);// "textures/fire", 8, 8, 0.05);
	std::cout << "shader_mesh_lights = " << shader_mesh_lights << std::endl;
}






void display_frame()
{
	// Update du temps avec le timer
	timer.update();
	float dt = timer.t - t;
	if (dt <= 0) dt += timer.t_max;
	t = t + dt;
	if (animationRuneActive)
		distance_zone += 1.2f * dt;
	else
		distance_zone -= 0.25f*(0.75f + 1.5f * user.keyboard_state.q) * dt;
	if (distance_zone < 0) distance_zone = 0;
	if (distance_zone > 12) distance_zone = 12;

	//distance_zone = 8;// scene.render_distance;


	if (animationRuneActive) {
		if (t - debutAnimationRune > 3.0f)
			animationRuneActive = false;
		sprite->update(dt);
		vcl::vec3 posPierre = terr->get_nearestPierre(loup->hierarchy["corps"].transform.translate);
		std::vector<vcl::vec3> result = terr->get_posOnTerrain(posPierre[0], posPierre[1]);
		sprite->set_pos(result[0] + vcl::vec3(0, 0, 0.2 + 0.38f*(1 - cos(2 * vcl::pi * (t - debutAnimationRune) / 3.0f))));
	}
	// camera
	/*
	if (user.keyboard_state.up) {
		scene.camera.position_camera += user.speed * 1.5f * dt * scene.camera.front();
	}
	else if (user.keyboard_state.down) {
		scene.camera.position_camera -= user.speed * 0.5f * dt * scene.camera.front();
	}
	scene.camera.position_camera[2] = user.height + terr->get_heightOfTerrain(scene.camera.position_camera[0], scene.camera.position_camera[1]);

	if (user.keyboard_state.a)
		scene.camera.manipulator_head_up(0.5f * dt);
	if (user.keyboard_state.q)
		scene.camera.manipulator_head_up(-0.5f * dt);
	if (user.keyboard_state.right)
		scene.camera.manipulator_turn(0.5f * dt);
	if (user.keyboard_state.left)
		scene.camera.manipulator_turn(-0.5f * dt);
	*/

	//scene.camera.orientation()


	// update de tous les objets qui ont besoin d'etre mis à jour
	/*
	pont->resetMasses();
	pont->ajouterMasse(0.5 + 0.5 * std::cos(0.3 * t), 0.5 + 0.2 * std::cos(6. * t), 0.25);
	float dt_pont = dt;
	float dt_max = 0.01;
	while (dt_pont >= dt_max) {
		pont->update(dt_max);
		dt_pont -= dt_max;
	}
	pont->update(dt_pont);
	*/
	//oiseau->update(dt);
	terr->update(dt);

	// On envoie les touches aux loup
	if (!(animationRuneActive || user.keyboard_state.a)) {
		loup->avancer = user.keyboard_state.up;
		loup->gauche = user.keyboard_state.left;
		loup->droite = user.keyboard_state.right;
	}
	else {
		loup->avancer = false;
		loup->gauche = false;
		loup->droite = false;
	}

	// On récupère la position du terrain pour placer le loup correctement (avec filtrage)
	float hChien = 0.73f;
	std::vector<vcl::vec3> posTerr = terr->get_posOnTerrain(loup->hierarchy["corps"].transform.translate[0], loup->hierarchy["corps"].transform.translate[1]);
	float ascension = posTerr[1][2] * produit_scal(vec3(sin(vcl::pi * loup->angleDirection / 180.0f), cos(-vcl::pi * loup->angleDirection / 180.0f), 0), posTerr[1]);
	bool peutAvancer = (ascension < 0.35f);
	loup->calcul_update(dt , peutAvancer);
	loup->hierarchy["corps"].transform.translate[2] += 0.3*(hChien + posTerr[0][2] - loup->hierarchy["corps"].transform.translate[2]);

	// Orientation du loup (avec filtrage)
	loup->hierarchy["corps"].transform.rotate = rotation::lerp(loup->hierarchy["corps"].transform.rotate, rotation_between_vector(vec3(0, 0, 1), vec3(0,1,0), posTerr[1], produit_vect(posTerr[1], vec3(cos(vcl::pi*loup->angleDirection/180.0f), sin(-vcl::pi*loup->angleDirection/180.0f),0))), 0.3);
	loup->hierarchy.update_local_to_global_coordinates();

	// On déplace la caméra (avec filtrage)
	camera_target_position += 0.2 * loup->vitesse *(loup->hierarchy["corps"].transform.translate - camera_target_position);      // position the camera is looking at / point around which the camera rotates
	camera_position += 0.2 * loup->vitesse * (camera_target_position + R * vcl::vec3(std::cos(vcl::pi * cam_angle_vertic / 180.0f) * std::sin(vcl::pi * (loup->angleDirection + cam_angle_horizont) / 180.0f),
		std::cos(vcl::pi * cam_angle_vertic / 180.0f) * std::cos(vcl::pi * (loup->angleDirection + cam_angle_horizont) / 180.0f),
		std::sin(vcl::pi * cam_angle_vertic / 180.0f)) - camera_position); // position of the camera in space
	vec3 const up = { 0,0,1 };                          // approximated "up" vector of the camera
	scene.camera.look_at(camera_position, camera_target_position, up);


	terr->posLoup = loup->hierarchy["corps"].transform.translate + loup->hierarchy["corps"].transform.rotate*vcl::vec3(0,-2,0);
	terr->eyesActivated = user.keyboard_state.q;




	// ================================ FIRST PASS =============================== //
	// First pass: Draw all shapes that cast shadows

	// On envoie les données aux vbo
	//glViewport(0, 0, scene.depth_map.width, scene.depth_map.height);
	//glBindFramebuffer(GL_FRAMEBUFFER, scene.depth_map.fbo); opengl_check;
	//glClear(GL_DEPTH_BUFFER_BIT);

	// Affichage de chacun des éléments
	//terr->afficher_depth_map(scene);
	//duck->afficher_depth_map(scene);
	//pont->afficher_depth_map(scene);
	//oiseau->afficher_depth_map(scene);
	//loup->afficher(scene);
	// ========================================================================== //


	//opengl_uniform(shader_mesh_lights, "pass", 1);  opengl_check;
	// ================================ SECOND PASS ============================= //
	// On ecrit dans le quadrangle
	//scene.filtrage = true;
	//separate = false;
	opengl_uniform(shader_mesh_lights, "red", rouge);  opengl_check;
	opengl_uniform(shader_mesh_lights, "green", vert);  opengl_check;
	opengl_uniform(shader_mesh_lights, "blue", bleu);  opengl_check;
	opengl_uniform(shader_mesh_lights, "dist_zone", distance_zone);  opengl_check;
	opengl_uniform(shader_mesh_lights, "interieur", false);  opengl_check;
	opengl_uniform(shader_mesh_lights, "posCentre", loup->hierarchy["corps"].transform.translate);  opengl_check;
	opengl_uniform(shader_mesh_lights, "filter", scene.filtrage);  opengl_check;
	opengl_uniform(shader_mesh_lights, "R", R);  opengl_check;

	if ((terr->has_pierre(loup->hierarchy["corps"].transform.translate)) && (user.keyboard_state.a) && !(animationRuneActive)) {
		animationRuneActive = true;
		debutAnimationRune = t;
		vcl::vec3 posPierre = terr->get_nearestPierre(loup->hierarchy["corps"].transform.translate);
		std::vector<vcl::vec3> result = terr->get_posOnTerrain(posPierre[0], posPierre[1]);
		sprite->set_pos(result[0] + vcl::vec3(0, 0, 0.2 + 0.38f * (1 - cos(2 * vcl::pi * (t - debutAnimationRune) / 3.0f))));
		sprite->set_rotation(vcl::rotation_between_vector(vcl::vec3(0, 1, 0), result[1]));
		sprite->reset();
	}
	
	if (!(separate)) {
	}
	else {
		rend.bind();

		// Affichage de chacun des éléments
		terr->afficher(scene, 0, scene.render_distance + 10);
		//terr->afficher(scene, distance_zone/sqrt(2), scene.render_distance + 8);
		//duck->afficher(scene);
		//pont->afficher(scene);

		//oiseau->afficher(scene);
		//loup->afficher(scene);
	}
	// ========================================================================== //



	// ================================== THIRD PASS ============================= //
	

	// Display the quad using the FBO texture, and use the shader applying the filter
	//rend2.draw_screen(scene); 
	// ========================================================================== //


	//scene.filtrage = false;
	rend2.bind();
	opengl_check;
	opengl_uniform(shader_mesh_lights, "interieur", true);  opengl_check;

	// Affichage de chacun des éléments
	terr->afficher(scene, 0, std::max(distance_zone + 13.0f, 15.0f));
	//duck->afficher(scene);
	//pont->afficher(scene);
	//oiseau->afficher(scene);
	loup->afficher(scene);
	if (animationRuneActive) sprite->afficher(scene);
	// ========================================================================== //



	// ================================== THIRD PASS ============================= //
	rend2.draw_screen(scene);
	if(separate)
		rend.draw_screen(scene);

	glDepthMask(GL_FALSE);
	draw_with_cubemap(cube, scene);
	glDepthMask(GL_TRUE);
}


void display_segment(vec3 const& a, vec3 const& b)
{
	segments.update({ a,b });
	draw(segments, scene);
}


void display_interface()
{
	ImGui::SliderFloat("Time", &timer.t, timer.t_min, timer.t_max);
	ImGui::SliderFloat("Time scale", &timer.scale, 0.0f, 2.0f);
	ImGui::Checkbox("Afficher lointain", &separate);
	ImGui::SliderFloat("Light falloff", &spotlight_falloff, 0, 1.0f, "%0.4f", 2.0f);
	ImGui::SliderFloat("Fog falloff", &scene.fog_falloff, 0, 1.0f, "%0.5f", 2.0f);
	ImGui::Checkbox("Filtrage", &scene.filtrage);
	ImGui::SliderFloat("Rouge", &rouge, 0, 1);
	ImGui::SliderFloat("Vert", &vert, 0,1);
	ImGui::SliderFloat("Bleu", &bleu, 0, 1);
	ImGui::SliderFloat("Distance Zone", &distance_zone, 0, 30);
}


void window_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
	float const aspect = width / static_cast<float>(height);
	scene.projection = projection_perspective(50.0f * pi / 180.0f, aspect, 0.1f, 100.0f);
}


void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*
	vec2 const  p1 = glfw_get_mouse_cursor(window, xpos, ypos);
	vec2 const& p0 = user.mouse_prev;
	glfw_state state = glfw_current_state(window);



	auto& camera = scene.camera;
	if (!user.cursor_on_gui) {
		if (state.mouse_click_left && !state.key_ctrl)
			scene.camera.manipulator_rotate_trackball(p0, p1);
		if (state.mouse_click_left && state.key_ctrl)
			camera.manipulator_translate_in_plane(p1 - p0);
		if (state.mouse_click_right) {
			//camera.manipulator_scale_distance_to_center((p1 - p0).y);
		}
	}

	user.mouse_prev = p1;
	*/

	vec2 const  p1 = glfw_get_mouse_cursor(window, xpos, ypos);
	vec2 const& p0 = user.mouse_prev;
	glfw_state state = glfw_current_state(window);


	auto& camera = scene.camera;
	if (!user.cursor_on_gui) {
		if (state.mouse_click_left) {
			cam_angle_horizont += (p1 - p0).x * 50;
			cam_angle_vertic -= (p1 - p0).y * 35;
		}
		//camera.manipulator_translate_in_plane(p1 - p0);
		if (state.mouse_click_right)
			R *= (1 + (p1 - p0).y * 5);
	}
	user.mouse_prev = p1;
}



