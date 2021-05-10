#pragma once
#include "vcl/vcl.hpp"
#include "scene_environment.hpp"

using namespace vcl;


class plane_renderer {

	public:
		mesh_drawable quad;
		GLuint fbo;                // Frame buffer for multi-pass render
		GLuint depth_buffer;       // Depth buffer used when rendered in the frame buffer
		GLuint intermediate_image; // Texture of the rendered color image
		int width;
		int height;

		plane_renderer();
		void initiate(GLuint shader);
		void initiate(GLuint shader, int w, int h);
		void initialize_fbo();
		void draw_screen(scene_environment& scene);
		void bind();
};