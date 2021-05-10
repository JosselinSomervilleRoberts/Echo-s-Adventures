#include "pont.hpp"
#include "utils.hpp"

Pont::Pont(vcl::vec3* ref11_, vcl::vec3* ref12_, vcl::vec3* ref21_, vcl::vec3* ref22_, int N_planches_, int N_cordes_, TextureLoader* textureLoader_) : AffichableUpdatable(textureLoader_) {
	// Paramètres
	N_planches = N_planches_;
	N_cordes = N_cordes_;
	K = 80;
	facteur_mult_noeuds = 3;

	// Références
	add_reference(ref11_);
	add_reference(ref12_);
	add_reference(ref21_);
	add_reference(ref22_);

	// Longueurs caractéristiques
	vecLargeur1 = (*references[2] - *references[0]);
	vecLongueur1 = (*references[1] - *references[0]);
	vecLargeur2 = (*references[3] - *references[1]);
	vecLongueur2 = (*references[3] - *references[2]);
	largeur1 = get_norme(vecLargeur1);
	L1 = get_norme(vecLongueur1);
	largeur2 = get_norme(vecLargeur2);
	L2 = get_norme(vecLongueur2);
	vecLargeur1 /= largeur1;
	vecLongueur1 /= L1;
	vecLargeur2 /= largeur2;
	vecLongueur2 /= L2;

	// Ne calcule pas vraiment les shapes mais obets (rondin, plache et corde)
	rayon_rondin = 0.2f * std::min(largeur1, largeur2);
	hauteur_rondin = 0.7f * std::min(largeur1, largeur2);
	largeur_planche = std::min(largeur1, largeur2);
	longueur_planche = 0.4f * std::min(L1, L2) / ((float)(N_planches));
	epaisseur_planche = 0.1f * std::min(L1, L2) / ((float)(N_planches));
	calcul_shape();

	// Cordes
	calculCoins();
	initiate_ropes();
	correctionPosition();
	calculReferencesSecondaire();
	initiate_secondary_ropes();
	equilibre();
}

Pont::~Pont() {
	delete rondin;
	delete planche;
	delete morceau_corde;
}


void Pont::calcul_shape() {
	// Meshes
	rondin = new Cylindre(rayon_rondin, hauteur_rondin, 0.1f, 20, textureLoader, "images/trunk.png", "images/tree-ring.png");
	morceau_corde = new Cylindre(0.2f, 1.02f, 0.1f, 8, textureLoader, "images/corde.png", "images/tree-ring.png");
	morceau_corde->set_display_disque(false);
	planche = new Box(largeur_planche / 2.0f * vecLargeur1, vecLongueur1 * longueur_planche, produit_vect(vecLargeur1, vecLongueur1) * epaisseur_planche, textureLoader, "images/planche.png");
}


void Pont::calculCoins() {
	int size = attache1.size();
	for (int i = 0; i < 4 + 2 * N_cordes - size; i++) {
		attache1.push_back({ 0.,0.,0. });
		attache2.push_back({ 0.,0.,0. });
	}

	for (int j = 0; j < 2; j++) {
		attache1[0 + 2 * j] = *references[0] + j * hauteur_rondin * vcl::vec3(0.0f, 0.0f, 0.8f);
		attache2[0 + 2 * j] = *references[1] + j * hauteur_rondin * vcl::vec3(0.0f, 0.0f, 0.8f);
		attache1[1 + 2 * j] = *references[2] + j * hauteur_rondin * vcl::vec3(0.0f, 0.0f, 0.8f);
		attache2[1 + 2 * j] = *references[3] + j * hauteur_rondin * vcl::vec3(0.0f, 0.0f, 0.8f);
	}
}

void Pont::initiate_ropes() {
	cordes.empty();
	int N = N_planches;

	for (int i = 0; i < 2; i++) {
		//if (i >= 2) N = N_cordes;
		Corde corde1(facteur_mult_noeuds * N);
		corde1.set_K(K);
		corde1.set_L(L1 * (0.8f / tension + 8.0f / (1.0f + K)));
		corde1.set_mu(0.1f);
		cordes.push_back(corde1);

		Corde corde2(facteur_mult_noeuds * N);
		corde2.set_K(K);
		corde2.set_L(L2 * (0.8f / tension + 8.0f / (1.0f + K)));
		corde2.set_mu(0.1f);
		cordes.push_back(corde2);
	}

	for (int i = 0; i < cordes.size(); i++) {
		cordes[i].fixerMorceau(0, &attache1[i]);
		cordes[i].fixerMorceau(cordes[i].get_N(), &attache2[i]);
	}
}


void Pont::initiate_secondary_ropes() {
	for (int i = 0; i < 2 * N_cordes; i++) {
		Corde corde(facteur_mult_noeuds * 4);
		corde.set_K(K);
		corde.set_L(hauteur_rondin * 0.8f);
		corde.set_mu(0.5f);
		cordes.push_back(corde);
	}

	for (int i = 4; i < cordes.size(); i++) {
		cordes[i].fixerMorceau(0, &attache1[i]);
		cordes[i].fixerMorceau(cordes[i].get_N(), &attache2[i]);
	}
}

void Pont::calculReferencesSecondaire() {
	int size = attache1.size();
	for (int i = 0; i < 4 + 2 * N_cordes - size; i++) {
		attache1.push_back({ 0.0f,0.0f,0.0f });
		attache2.push_back({ 0.0f,0.0f,0.0f });
	}

	for (int j = 0; j < N_cordes; j++) {
		attache1[4 + 2 * j] = cordes[2].getPos((0.5f + j) / ((float)(N_cordes)));
		int index = facteur_mult_noeuds * N_planches * (0.5f + j) / ((float)(N_cordes));
		attache2[4 + 2 * j] = pos_corde_1[index] + (facteur_mult_noeuds * N_planches * (0.5f + j) / ((float)(N_cordes)) - index) * (pos_corde_1[index + 1] - pos_corde_1[index]);
		attache1[5 + 2 * j] = cordes[3].getPos((0.5f + j) / ((float)(N_cordes)));
		attache2[5 + 2 * j] = pos_corde_2[index] + (facteur_mult_noeuds * N_planches * (0.5f + j) / ((float)(N_cordes)) - index) * (pos_corde_1[index + 1] - pos_corde_1[index]);
	}
}


void Pont::ajouterMasse(float u, float v, float m) {

	cordes[0].ajouterMasse(u, m * (1. - v), (int)(N_planches / 2.0f));
	cordes[1].ajouterMasse(u, m * v, (int)(N_planches / 2.0f));
	needsToBeUpdated = true;
}


void Pont::resetMasses() {
	cordes[0].resetMasses();
	cordes[1].resetMasses();
	needsToBeUpdated = true;
}

void Pont::equilibre() {
	calculCoins();
	for (int i = 0; i < 4; i++)
		cordes[i].equilibre();
	correctionPosition();
	calculReferencesSecondaire();

	for (int i = 4; i < cordes.size(); i++)
		cordes[i].repartir();
}

void Pont::correctionPosition() {
	// On assure la distance de largeur entre les 2 cordes
	pos_corde_1 = cordes[0].getPositions();
	pos_corde_2 = cordes[1].getPositions();
	for (int i = 0; i <= pos_corde_1.size(); i++) {
		float dh = pos_corde_1[i][2] - pos_corde_2[i][2];
		float dx = 0.5 * (largeur_planche - sqrt(pow(largeur_planche, 2) - pow(dh, 2)));
		//std::cout << dx << " / " << largeur << std::endl;
		pos_corde_1[i] -= dx * vecLargeur1;
		pos_corde_2[i] += dx * vecLargeur2;
	}
}


float Pont::calcul_update(float dt) {
	calculCoins();
	float mouvMax = 0;

	// Cordes de soutien des planches
	std::vector<vcl::vec3> forces_ext;
	std::vector<vcl::vec3> forces_ext_rampes;
	for (int i = 0; i <= cordes[0].get_N(); i++) {
		forces_ext.push_back(vcl::vec3(0.0f, 0.0f, 0.0f));
		forces_ext_rampes.push_back(vcl::vec3(0.0f, 0.0f, 0.0f));
	}

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < N_cordes; i++) {
			float allongement = cordes[4 + j + 2 * i].getLastLongueur() - cordes[4 + j + 2 * i].get_L0();
			vcl::vec3 vect_dir_descendant = normalize(cordes[4 + j + 2 * i].getPositions()[cordes[4 + j + 2 * i].get_N()] - cordes[4 + j + 2 * i].getPositions()[cordes[4 + j + 2 * i].get_N() - 1]);
			if ((allongement > 0) && (produit_scal(vect_dir_descendant, vcl::vec3(0., 0., -1.)) > 0)) {
				float intensite = 0.5f * K * allongement;
				float index_float = facteur_mult_noeuds * N_planches * (0.5f + j) / ((float)(N_cordes));
				int index = index_float;
				for (int ii = std::max(0, index - 3); ii <= std::min(index + 3, cordes[4 + j + 2 * i].get_N()); ii++) {
					forces_ext[ii] = -intensite * gaussian(ii, index_float, 2) * vect_dir_descendant;
					forces_ext_rampes[ii] = +intensite * gaussian(ii, index_float, 2) * vect_dir_descendant;
				}
			}
		}
	}
	for (int i = 0; i < 2; i++)
		mouvMax = std::max(mouvMax, cordes[i].calcul_update(dt, forces_ext));
	correctionPosition();

	// Les cordes de "rampe"
	for (int i = 2; i < 4; i++)
		mouvMax = std::max(mouvMax, cordes[i].calcul_update(dt, forces_ext_rampes));

	// Toutes les cordes secondaires
	calculReferencesSecondaire();
	for (int i = 4; i < cordes.size(); i++)
		mouvMax = std::max(mouvMax, cordes[i].calcul_update(dt));
	return mouvMax;
}


void Pont::afficher(scene_environment& scene) {
	for (int i = 0; i < 2; i++) {
		rondin->set_pos(attache1[i] - rondin->get_r() * vecLongueur1);
		rondin->afficher(scene);
		rondin->set_pos(attache2[i] + rondin->get_r() * vecLongueur1);
		rondin->afficher(scene);
	}

	// Affichage des 2 cordes de soutien
	for (int i = 0; i <= pos_corde_1.size(); i++) {
		if ((i + int(facteur_mult_noeuds / 2.0)) % facteur_mult_noeuds == 0) {
			vcl::vec3 vect1 = vecLargeur1;
			vcl::vec3 vect2 = vecLongueur1;
			vcl::vec3 vect1_trans = normalize(pos_corde_2[i] - pos_corde_1[i]);
			vcl::vec3 vect2_trans = normalize((pos_corde_1[i + 1] + pos_corde_2[i + 1]) / 2.0 - (pos_corde_1[i - 1] + pos_corde_2[i - 1]) / 2.0);
			vect2_trans -= produit_scal(vect1_trans, vect2_trans) * vect1_trans;
			vect2_trans = normalize(vect2_trans);
			vcl::rotation r = rotation_between_vector(vect1, vect2, vect1_trans, vect2_trans);

			planche->set_rotation(r);
			planche->set_pos((pos_corde_1[i] + pos_corde_2[i]) / 2.0);
			planche->afficher(scene);
		}

		if ((i > 0) && (i < pos_corde_1.size())) {
			morceau_corde->set_pos(pos_corde_1[i]);

			vcl::vec3 vect1 = vecLargeur1;
			vcl::vec3 vect2 = vcl::vec3(0, 0, -1.);
			vcl::vec3 vect1_trans = pos_corde_2[i] - pos_corde_1[i];
			vect1_trans = normalize(vect1_trans);
			vcl::vec3 vect2_trans = normalize(pos_corde_1[i] - pos_corde_1[i - 1]);
			vect2_trans -= produit_scal(vect1_trans, vect2_trans) * vect1_trans;
			vect2_trans = normalize(vect2_trans);
			vcl::rotation r = rotation_between_vector(vect1, vect2, vect1_trans, vect2_trans);

			morceau_corde->set_rotation(r);
			morceau_corde->set_scale(get_norme(pos_corde_1[i] - pos_corde_1[i - 1]));
			morceau_corde->afficher(scene);


			vect2_trans = normalize(pos_corde_2[i] - pos_corde_2[i - 1]);
			vect2_trans -= produit_scal(vect1_trans, vect2_trans) * vect1_trans;
			vect2_trans = normalize(vect2_trans);
			r = rotation_between_vector(vect1, vect2, vect1_trans, vect2_trans);

			morceau_corde->set_pos(pos_corde_2[i]);
			morceau_corde->set_rotation(r);
			morceau_corde->set_scale(get_norme(pos_corde_2[i] - pos_corde_2[i - 1]));
			morceau_corde->afficher(scene);
		}
	}

	for (int j = 2; j < cordes.size(); j++) {
		std::vector<vcl::vec3>positions_corde = cordes[j].getPositions();
		for (int i = 0; i <= cordes[j].get_N(); i++) {

			if ((i > 0) && (i < positions_corde.size())) {
				vcl::vec3 vect1 = vecLargeur1;
				vcl::vec3 vect2 = vcl::vec3(0, 0, -1.);
				vcl::vec3 vect1_trans = vecLargeur1;
				vcl::vec3 vect2_trans = normalize(positions_corde[i] - positions_corde[i - 1]);
				vect2_trans -= produit_scal(vect1_trans, vect2_trans) * vect1_trans;
				vect2_trans = normalize(vect2_trans);
				vcl::rotation r = rotation_between_vector(vect1, vect2, vect1_trans, vect2_trans);

				morceau_corde->set_pos(positions_corde[i]);
				morceau_corde->set_rotation(r);
				morceau_corde->set_scale(get_norme(positions_corde[i] - positions_corde[i - 1]));
				morceau_corde->afficher(scene);
			}
		}
	}
}