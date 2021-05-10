#include "plane_renderer.hpp"
using namespace vcl;


plane_renderer::plane_renderer() {
	this->fbo = 0;                // Frame buffer for multi-pass render
	this->depth_buffer = 0;       // Depth buffer used when rendered in the frame buffer
	this->intermediate_image = 0; // Texture of the rendered color image
	this->width = 1920;
	this->height = 1000;
}


void plane_renderer::initiate(GLuint shader) {
	initialize_fbo();

	//this->quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,0 }, { 0,-1,0 }, { 0,1,0 }, { -1,1,0 }));
	this->quad = mesh_drawable(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	this->quad.texture = this->intermediate_image;
	this->quad.shader = shader;
};


void plane_renderer::initiate(GLuint shader, int w, int h) {
	this->width = w;
	this->height = h;
	initiate(shader);
}


void plane_renderer::initialize_fbo()
{
	// Allocate texture to store the rendered image
	glGenTextures(1, &(this->intermediate_image));
	glBindTexture(GL_TEXTURE_2D, this->intermediate_image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Allocate a depth buffer when using the frame buffer
	glGenRenderbuffers(1, &(this->depth_buffer));
	glBindRenderbuffer(GL_RENDERBUFFER, this->depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);

	// Create frame buffer
	glGenFramebuffers(1, &(this->fbo));
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	// associate the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->intermediate_image, 0);
	// associate the depth-buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_buffer);
}



void plane_renderer::draw_screen(scene_environment& scene)
{
	// Display the quad using the FBO texture, and use the shader
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	// Setup shader
	assert_vcl(this->quad.shader != 0, "Try to draw mesh_drawable without shader");
	assert_vcl(this->quad.texture != 0, "Try to draw mesh_drawable without texture");
	glUseProgram(this->quad.shader); opengl_check;

	// Set texture
	glActiveTexture(GL_TEXTURE0); opengl_check;
	glBindTexture(GL_TEXTURE_2D, this->quad.texture); opengl_check;
	opengl_uniform(this->quad.shader, "image_texture", 0, false);  opengl_check;
	opengl_uniform(this->quad.shader, "filter", scene.filtrage);  opengl_check;

	// Call draw function
	assert_vcl(this->quad.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles"); opengl_check;
	glBindVertexArray(this->quad.vao);   opengl_check;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quad.vbo.at("index")); opengl_check;
	glDrawElements(GL_TRIANGLES, GLsizei(this->quad.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

	// Clean buffers
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void plane_renderer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	//glViewport(0, 0, scene.window_width, scene.window_height);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}