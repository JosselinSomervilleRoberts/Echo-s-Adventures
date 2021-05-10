#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "textureLoader.hpp"
#include "utils.hpp"
#include "interpolation.hpp"

#include <iostream>
#include  <list>


class Side{

	public:
		bool constrained;
		std::vector<float> positions; 
		std::vector<float> derivatives;

		Side();
		Side(size_t N);
		Side(std::vector<float> &positions_, std::vector<float> &derivatives_);
		Side(size_t N, float pos0, float der0, float pos1, float der1);
		void constain_side(std::vector<float>& positions_, std::vector<float>& derivatives_);
		void generate_side(size_t N, float pos0, float der0, float pos1, float der1);

		Side reversed();

		Side operator-(const Side& s);
};