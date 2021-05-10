#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "utils.hpp"

shadow_map_parameters initialize_depth_map();
void draw_with_shadow(vcl::mesh_drawable const& drawable, scene_environment const& scene);
void draw_depth_map(vcl::mesh_drawable const& drawable, scene_environment const& current_scene);
//void opengl_uniform(GLuint shader, scene_environment const& current_scene);