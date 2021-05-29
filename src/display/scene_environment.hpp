#pragma once
#include "vcl/vcl.hpp"
#include "camera_custom.hpp"
#include <iostream>
#include <list>

using namespace vcl;


struct shadow_map_parameters
{
	GLuint texture;
	int width;
	int height;
	GLuint fbo;
	GLuint shader;
};


struct scene_environment
{
	camera_around_center camera;
	mat4 projection;

	// Consider a set of spotlight defined by their position and color
	std::vector<vec3> spotlight_position;
	std::vector<vec3> spotlight_color;
	std::vector<float> spotlight_falloff;
	std::vector<float> spotlight_intensity;
	int N_lights = 0;
	float fog_falloff = 0.01f;
	float render_distance = 15;

	// Differents modes d'affichage
	int mode = 0;
	bool filtrage = false;

	// Ombres
	vcl::camera_spherical_coordinates light;
	vcl::mat4 projection_light;
	int window_width, window_height;
	shadow_map_parameters depth_map;


	void ajouterLumiere(vcl::vec3 pos, vcl::vec3 color, float falloff, float intensity) {
		N_lights++;
		spotlight_position.push_back(pos);
		spotlight_color.push_back(color);
		spotlight_falloff.push_back(falloff);
		spotlight_intensity.push_back(intensity);
	};

	void retirerLumiere(int index) {
		if (index < N_lights) {
			N_lights--;
			spotlight_position.erase(spotlight_position.begin() + index);
			spotlight_color.erase(spotlight_color.begin() + index);
			spotlight_falloff.erase(spotlight_falloff.begin() + index);
			spotlight_intensity.erase(spotlight_intensity.begin() + index);
		}
	};

	void clearLumiere() {
		N_lights = 0;
		spotlight_position.clear();
		spotlight_color.clear();
		spotlight_falloff.clear();
		spotlight_intensity.clear();
	};
};