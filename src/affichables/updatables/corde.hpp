#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "affichableUpdatable.hpp"
#include <iostream>
#include <list>


class Corde : public AffichableUpdatable {

	private:
		// Grandeurs pour le calcul de la position
		std::vector<vcl::vec3> positions;
		std::vector<vcl::vec3> vitesses;
		std::vector<float> masses_additionnelles;
		std::vector<bool> positions_fixes;

		// Paramètres
		float L;
		int N;
		int ordre;
		float m;
		float K;
		float mu;
		float L0;

		vcl::vec3 spring_force(vcl::vec3 const& p_i, vcl::vec3 const& p_j, float L0_, float K_);

		vcl::mesh mesh_corde;

	public:
		void set_L(float L_) { L = L_; L0 = L / (float)(N); };
		void set_ordre(int ordre_) { ordre = ordre_; };
		void set_m(float m_) { m = m_; };
		void set_K(float K_) { K = K_; };
		void set_mu(float mu_) { L = mu_; };

		float get_L() { return L; };
		float get_L0() { return L0; };
		int get_N() { return N; };
		int get_ordre() { return ordre; };
		float get_m() { return m; };
		float get_K() { return K; };
		float get_mu() { return mu; };

		Corde();
		Corde(int N_);
		void repartir();
		void equilibre();
		std::vector<vcl::vec3> getPositions();
		void libererMorceau(int index);
		void fixerMorceau(int index, vcl::vec3* position);
		void ajouterMasse(float u, float masse_, float largeur);
		void resetMasses();
		float getCurrentLongueur();
		vcl::vec3 getPos(float u);
		float getLastLongueur();

		void afficher(scene_environment& scene);
		void calcul_shape() {};
		float calcul_update(float dt);
		float calcul_update(float dt, std::vector<vcl::vec3> forces_exterieures);
};