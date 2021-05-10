#pragma once

#include "vcl/vcl.hpp"
#include "corde.hpp"
#include "scene_environment.hpp"
#include "cylindre.hpp"
#include "box.hpp"
#include "textureloader.hpp"

#include <iostream>
#include <list>


class Pont : public AffichableUpdatable  {
	private:
		std::vector<Corde> cordes;

		// Paramètres
		float L1;
		float L2;
		float largeur1;
		float largeur2;
		int N_planches;
		int facteur_mult_noeuds;
		int N_cordes;
		float K;

		// Pour les objets que l'ona ffiche
		float rayon_rondin;
		float hauteur_rondin;
		float largeur_planche;
		float longueur_planche;
		float epaisseur_planche;

		float tension = 2.0f;

		std::vector<vcl::vec3> attache1;
		std::vector<vcl::vec3> attache2;
		vcl::vec3 vecLargeur1;
		vcl::vec3 vecLongueur1;
		vcl::vec3 vecLargeur2;
		vcl::vec3 vecLongueur2;

		Cylindre *rondin;
		Cylindre *morceau_corde;
		Box* planche;

		// Positions corrigées des cordes
		std::vector<vcl::vec3>pos_corde_1;
		std::vector<vcl::vec3>pos_corde_2;

	public:
		void set_K(float K_) { K = K_; };
		float get_K() { return K; };

		Pont(vcl::vec3* ref11_, vcl::vec3* ref12_, vcl::vec3* ref21_, vcl::vec3* ref22_,  int N_planches_, int N_cordes_, TextureLoader* textureLoader_);
		~Pont();
		void calculCoins();
		void initiate_ropes();
		void calculReferencesSecondaire();
		void initiate_secondary_ropes();
		void equilibre();
		void ajouterMasse(float u, float v, float m);
		void resetMasses();
		void correctionPosition();

		float calcul_update(float dt);
		void calcul_shape();
		void afficher(scene_environment& scene);
};