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
	camera_custom camera;
	mat4 projection;

	// Consider a set of spotlight defined by their position and color
	std::array<vec3, 5> spotlight_position;
	std::array<vec3, 5> spotlight_color;
	float spotlight_falloff = 0.001;
	float fog_falloff = 0.001f;

	// Differents modes d'affichage
	int mode = 0;
	bool filtrage = false;

	// Ombres
	vcl::camera_spherical_coordinates light;
	vcl::mat4 projection_light;
	int window_width, window_height;
	shadow_map_parameters depth_map;
};