#pragma once

#include "affichable.hpp"
#include <list>
#include <stdlib.h>

class Cylindre : public Affichable {

	protected:
		double r;
		double h;
		double noise;
		size_t N;

		vcl::vec3 normal;

	public:
		Cylindre(double r_, double h_);
		Cylindre(double r_, double h_, double noise_);
		Cylindre(double r_, double h_, double noise_, size_t N_);
		Cylindre(double r_, double h_, double noise_, TextureLoader* textureLoader_,  std::string name_texture_cylindre, std::string name_texture_disque);
		Cylindre(double r_, double h_, double noise_, size_t N_, TextureLoader* textureLoader_, std::string name_texture_cylindre, std::string name_texture_disque);

		double get_r() { return r; };
		void set_display_disque(bool b);

		void calcul_disque();
		void calcul_cylindre();
		void calcul_shape();
		void afficher(scene_environment& scene);
};