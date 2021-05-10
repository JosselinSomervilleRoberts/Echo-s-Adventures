#pragma once

#include "vcl/math/math.hpp"
#include "vcl/interaction/camera/camera_base/camera_base.hpp"


using namespace vcl;

	struct camera_custom : camera_base
	{
		camera_custom();

		rotation orientation_camera;
		vec3 position_camera;
		vec3 orientation_vec;
		vec3 orientation_vec_second;

		vec3 position() const;
		rotation orientation() const;

		void manipulator_turn(float angle);
		void manipulator_head_up(float angle);
		void update_orientation();
		void manipulator_rotate_trackball(vec2 const& p0, vec2 const& p1);
		void manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw);
		void manipulator_translate_in_plane(vec2 const& tr);
	};
