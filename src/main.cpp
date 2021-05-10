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






plane_renderer rend;
plane_renderer rend2;
float rouge = 1;
float vert = 1;
float bleu = 1;
float distance_zone = 8;


// Store keyboard state
// left-right / up-down key
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



int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	width = 1920, height = 1000;
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
	GLuint texture_cubemap = cubemap_texture("images/skybox/");

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
	scene.camera.position_camera = vcl::vec3(0, 0, 0);
	scene.camera.manipulator_rotate_roll_pitch_yaw(0, vcl::pi / 2.0f, 0);

	// Pour les ombres
	scene.depth_map = initialize_depth_map();
	scene.projection_light = projection_orthographic(-10, 10, -10, 10, 0, 30); // orthographic projection for simplicity
	scene.light.distance_to_center = 5.0f;
	scene.light.manipulator_rotate_spherical_coordinates(pi / 4.0f, pi / 4.0f);




	// Création du terrain
	int nb_chunks = 15;
	int N = 20;
	float chunk_size = 2;
	terr = new Terrain(chunk_size, N, &textureLoader);
	scene.camera.position_camera[2] = user.height + terr->get_heightOfTerrain(scene.camera.position_camera[0], scene.camera.position_camera[1]);


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

	// Key positions
	int N_steps = 20;
	int R = 18;
	for (int i = 0; i < N_steps + 1; i++) {
		oiseau->key_positions.push_back({ R * std::sin(3.14f * 2 * i / float(N_steps)), R * std::cos(3.14f * 2 * i / float(N_steps)), 14 + double(int(rand() % 30)) / 20.0 });
		oiseau->key_times.push_back(double(i * 2.2));
	}

	// Set timer bounds
	//  You should adapt these extremal values to the type of interpolation
	size_t const N2 = oiseau->key_times.size();
	timer.t_min = oiseau->key_times[0];    // Start the timer at the first time of the keyframe
	timer.t_max = oiseau->key_times[N2 - 1];  // Ends the timer at the last time of the keyframe
	timer.t = timer.t_min;

	// Création du canard
	duck = new ObjModel("house1", &textureLoader);
	duck->set_pos(vcl::vec3(-8.4, 3, 0.7 * 2 + std::max(terr->get_heightOfTerrain(-8, 3), terr->get_heightOfTerrain(-8, 5))));
	duck->set_rotation(vcl::rotation(vcl::vec3(0, 0, 1), vcl::pi / 2.0f));





	// Shader computing an image filter in the second pass
	GLuint const shader_screen_render = opengl_create_shader_program(read_text_file("shader/screen_render.vert.glsl"), read_text_file("shader/screen_render.frag.glsl"));
	GLuint const shader_screen_render2 = opengl_create_shader_program(read_text_file("shader/screen_render2.vert.glsl"), read_text_file("shader/screen_render2.frag.glsl"));

	// The result of the second pass is displayed on a quad
	//rend.quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	//rend2.quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,-1 }, { 0,-1,-1 }, { 0,1,-1 }, { -1,1,-1 }));
	rend.initiate(shader_screen_render, width, height);
	rend2.initiate(shader_screen_render2, width, height);
}






void display_frame()
{
	// Update du temps avec le timer
	timer.update();
	float dt = timer.t - t;
	if (dt <= 0) dt += timer.t_max;
	t = t + dt;


	// camera
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

	//scene.camera.orientation()


	// update de tous les objets qui ont besoin d'etre mis à jour
	pont->resetMasses();
	pont->ajouterMasse(0.5 + 0.5 * std::cos(0.3 * t), 0.5 + 0.2 * std::cos(6. * t), 0.25);
	float dt_pont = dt;
	float dt_max = 0.01;
	while (dt_pont >= dt_max) {
		pont->update(dt_max);
		dt_pont -= dt_max;
	}
	pont->update(dt_pont);
	oiseau->update(dt);
	loup->update(dt);

	scene.spotlight_color[0] = { 1.0f, 0.0f, 0.0f };
	scene.spotlight_position[0] = { std::cos(t), std::sin(t), 0.5 + 0.2 * std::cos(3 * t) };

	scene.spotlight_color[1] = { 1.0f, 0.0f, 0.0f };
	scene.spotlight_position[1] = { 10 * std::cos(0.5 * t + pi / 2), 10 * std::sin(0.5 * t + pi / 2), 15 };

	scene.spotlight_position[2] = { 0,0,1.05f };
	scene.spotlight_color[2] = 2 * (std::cos(t) + 1.0f) / 2.0 * vec3(1, 1, 1);

	scene.spotlight_position[3] = { 3 * std::cos(t), 3 * std::sin(t), 0.5 + 0.2 * std::cos(3 * t) };
	scene.spotlight_color[3] = vec3((std::cos(t) + 1) / 2, 0, 1);

	scene.spotlight_position[4] = { -3.0f,-3.0f,0.05f };
	scene.spotlight_color[4] = { 1.0f, 0.9f, 0.5f };



	//opengl_uniform(3, "pass", 2);  opengl_check;
	//terr->afficher(scene);



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
	bool separate = false;
	
	if (!(separate)) {
		//distance_zone = 30.0f;
		opengl_uniform(shader_mesh_lights, "red", rouge);  opengl_check;
		opengl_uniform(shader_mesh_lights, "green", vert);  opengl_check;
		opengl_uniform(shader_mesh_lights, "blue", bleu);  opengl_check;
		opengl_uniform(shader_mesh_lights, "dist_zone", distance_zone);  opengl_check;
		opengl_uniform(shader_mesh_lights, "posCam", scene.camera.position());
		opengl_uniform(shader_mesh_lights, "filter", scene.filtrage);
	}
	else {
		rend.bind();
		opengl_uniform(shader_mesh_lights, "red", rouge);  opengl_check;
		opengl_uniform(shader_mesh_lights, "green", vert);  opengl_check;
		opengl_uniform(shader_mesh_lights, "blue", bleu);  opengl_check;
		opengl_uniform(shader_mesh_lights, "dist_zone", distance_zone);  opengl_check;
		opengl_uniform(shader_mesh_lights, "interieur", false);  opengl_check;
		opengl_uniform(shader_mesh_lights, "posCam", scene.camera.position());
		opengl_uniform(shader_mesh_lights, "filter", scene.filtrage);

		// Affichage du fond (skybox)
		glDepthMask(GL_FALSE);
		//draw_with_cubemap(cube, scene);
		glDepthMask(GL_TRUE);

		// Affichage de chacun des éléments
		terr->afficher(scene);
		duck->afficher(scene);
		pont->afficher(scene);
		oiseau->afficher(scene);
		//loup->afficher_with_shadow(scene);
	}
	// ========================================================================== //



	// ================================== THIRD PASS ============================= //
	

	// Display the quad using the FBO texture, and use the shader applying the filter
	//rend2.draw_screen(scene);
	// ========================================================================== //


	//scene.filtrage = false;
	rend2.bind();
	//opengl_uniform(shader_mesh_lights, "red", 1.0f);  opengl_check;
	//opengl_uniform(shader_mesh_lights, "green", 0.0f);  opengl_check;
	//opengl_uniform(shader_mesh_lights, "blue", 0.0f);  opengl_check;
	opengl_uniform(shader_mesh_lights, "interieur", true);  opengl_check;
	//opengl_uniform(shader_mesh_lights, "posCam", scene.camera.position());
	//opengl_uniform(shader_mesh_lights, "filter", scene.filtrage);

	// Affichage du fond (skybox)
	glDepthMask(GL_FALSE);
	////draw_with_cubemap(cube, scene);
	glDepthMask(GL_TRUE);

	// Affichage de chacun des éléments
	terr->afficher(scene);
	duck->afficher(scene);
	pont->afficher(scene);
	oiseau->afficher(scene);
	//loup->afficher_with_shadow(scene);
	// ========================================================================== //



	// ================================== THIRD PASS ============================= //


	// Display the quad using the FBO texture, and use the shader applying the filter
	//rend.quad.transform.translate = vcl::vec3(0, 0, -1);
	rend2.draw_screen(scene);
	if(separate)
		rend.draw_screen(scene);



	/*

	//opengl_uniform(shader_mesh_lights, "pass", 2);  opengl_check;
	// ================================ SECOND PASS ============================= //
	// On ecrit dans le quadrangle
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glViewport(0, 0, scene.window_width, scene.window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	opengl_uniform(3, "mode", 0);  opengl_check;

	// Affichage du fond (skybox)
	glDepthMask(GL_FALSE);
	draw_with_cubemap(cube, scene);
	glDepthMask(GL_TRUE);

	// Affichage de chacun des éléments
	terr->afficher(scene);
	duck->afficher(scene);
	pont->afficher(scene);
	oiseau->afficher(scene);
	//loup->afficher_with_shadow(scene);
	// ========================================================================== //



	// ================================== THIRD PASS ============================= //
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Display the quad using the FBO texture, and use the shader applying the filter
	draw_screen(quad);
	// ========================================================================== //
	*/

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
	ImGui::Checkbox("Frame", &user.gui.display_frame);
	ImGui::SliderFloat("Light falloff", &scene.spotlight_falloff, 0, 1.0f, "%0.4f", 2.0f);
	ImGui::SliderFloat("Fog falloff", &scene.fog_falloff, 0, 0.05f, "%0.5f", 2.0f);
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
}



