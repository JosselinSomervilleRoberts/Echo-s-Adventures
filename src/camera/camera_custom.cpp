#include "camera_custom.hpp"

#include "vcl/base/base.hpp"
#include "vcl/interaction/camera/common_functions/common_functions.hpp"

using namespace vcl;


inline vcl::vec3 produit_vect_redef(vcl::vec3 a, vcl::vec3 b) {
	return vcl::vec3(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}


	camera_custom::camera_custom()
		:orientation_camera(), position_camera()
	{
		orientation_vec = normalize(vec3(0, 1, 5));
		update_orientation();
	}

	void camera_custom::update_orientation() {
		vec3 v1_before = vec3(0, 1, 0);
		vec3 v2_before = vec3(-1, 0, 0);
		vec3 v1_after = orientation_vec;
		orientation_vec_second = produit_vect_redef(vcl::vec3(0, 0, 1), v1_after);
		orientation_vec_second = normalize(orientation_vec_second);
		vec3 v2_after = orientation_vec_second;
		orientation_camera = rotation_between_vector(v1_before, v2_before, v1_after, v2_after);
	}

	vec3 camera_custom::position() const
	{
		return position_camera;
	}
	rotation camera_custom::orientation() const
	{
		return orientation_camera;
	}

	void camera_custom::manipulator_rotate_trackball(vec2 const& p0, vec2 const& p1)
	{
		rotation const r = trackball_rotation(p0, p1);
		orientation_camera = orientation_camera * r;
	}
	void camera_custom::manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw)
	{
		rotation r_roll = rotation({ 0,0,-1 }, roll);
		rotation r_pitch = rotation({ 1,0,0 }, pitch);
		rotation r_yaw = rotation({ 0,1,0 }, yaw);

		orientation_camera = orientation_camera * r_roll * r_pitch * r_yaw;
	}
	void camera_custom::manipulator_translate_in_plane(vec2 const& tr)
	{
		position_camera -= translation_in_plane(tr, orientation());
	}


	void camera_custom::manipulator_turn(float angle) {
		orientation_vec = rotation(vcl::vec3(0, 0, 1), angle) * orientation_vec;
		update_orientation();
	}

	void camera_custom::manipulator_head_up(float angle) {
		orientation_vec = rotation(orientation_vec_second, angle) * orientation_vec;
		update_orientation();
	}

