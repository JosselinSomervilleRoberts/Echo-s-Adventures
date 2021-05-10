#include "side.hpp"

Side::Side() {
	constrained = false;
}

Side::Side(size_t N) {
	positions.clear();
	derivatives.clear();
	for (int i = 0; i < N; i++) {
		positions.push_back(0);
		derivatives.push_back(0);
	}
	constrained = false;
}

Side::Side(std::vector<float>& positions_, std::vector<float>& derivatives_) {
	constain_side(positions_, derivatives_);
}

Side::Side(size_t N, float pos0, float der0, float pos1, float der1) {
	generate_side(N, pos0, der0, pos1, der1);
}

void Side::constain_side(std::vector<float> &positions_, std::vector<float> &derivatives_) {
	constrained = true;
	positions.clear();
	derivatives.clear();
	for (int i = 0; i < positions_.size(); i++) {
		positions.push_back(positions_[i]);
		derivatives.push_back(derivatives_[i]);
	}
}

void Side::generate_side(size_t N, float pos0, float der0, float pos1, float der1) {
	constrained = false;
	positions.clear();
	derivatives.clear();
	for (int i = 0; i < N; i++) {
		positions.push_back(interpolation2((float)(i) / (float)(N-1), pos0, der0, pos1, der1));
		derivatives.push_back(interpolation2_derivative((float)(i) / (float)(N - 1), pos0, der0, pos1, der1));
	}
}


Side Side::reversed() {
	Side s;
	s.constrained = constrained;

	for (int i = positions.size() - 1; i >= 0; i--) {
		s.positions.push_back(positions[i]);
		s.derivatives.push_back(derivatives[i]);
	}
	return s;
}


Side Side::operator-(const Side& s) {
	Side new_s;
	new_s.constrained = constrained || s.constrained;
	for (int i = 0; i < positions.size(); i++) {
		new_s.positions.push_back(positions[i] - s.positions[i]);
		new_s.derivatives.push_back(derivatives[i] - s.derivatives[i]);
	}
	return new_s;
}