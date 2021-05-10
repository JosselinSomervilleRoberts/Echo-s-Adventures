#include "corde.hpp"
#include "utils.hpp"

Corde::Corde(int N_) : AffichableUpdatable() {
	N = N_;
	ordre = 1;
	L = 5.0f;
	m = 0.01f;        // particle mass
	K = 20.0f;        // spring stiffness
	mu = 0.02f;       // damping coefficient
	L0 = L / (float)(N);

	for (int i = 0; i <= N; i++) {
		positions.push_back({ 0.0f, i * L0 / (float)(N), 0.0f });
		vitesses.push_back({ 0.0f,0.0f,0.0f });
		positions_fixes.push_back(false);
		references.push_back(NULL);
		masses_additionnelles.push_back(0.0f);
	}
}


Corde::Corde() : AffichableUpdatable() {
	N = 20;
	ordre = 1;
	L = 5.0f;
	m = 0.01f;        // particle mass
	K = 20.0f;        // spring stiffness
	mu = 0.02f;       // damping coefficient
	L0 = L / (float)(N);

	for (int i = 0; i <= N; i++) {
		positions.push_back({ 0.0f, i * L0 / (float)(N), 0.0f });
		vitesses.push_back({ 0.0f,0.0f,0.0f });
		positions_fixes.push_back(false);
		references.push_back(NULL);
		masses_additionnelles.push_back(0.0f);
	}
}


vcl::vec3 Corde::spring_force(vcl::vec3 const& p_i, vcl::vec3 const& p_j, float L0_, float K_)
{
	vcl::vec3 vecteur = p_i - p_j;
	float norme = sqrt(pow(vecteur[0], 2) + pow(vecteur[1], 2) + pow(vecteur[2], 2));
	return -K_ * vecteur * (1 - L0_ / norme);
}


float Corde::calcul_update(float dt) {
	std::vector<vcl::vec3> forces_exterieures;
	for(int i=0; i<=N; i++)
		forces_exterieures.push_back(vcl::vec3(0.0f,0.0f,0.0f));
	return calcul_update(dt, forces_exterieures);
}

float Corde::calcul_update(float dt, std::vector<vcl::vec3> forces_exterieures) {
	float mouvMax = 0;

	vcl::vec3 const g = { 0,0,-9.81f }; // gravity
	std::vector<vcl::vec3> liste_forces;

	// Update des références
	for (int i = 0; i <= N; i++) {
		if (positions_fixes[i]) {
			mouvMax = std::max(mouvMax, get_norme(*references[i] - positions[i]));
			positions[i] = *references[i];
		}
	}

	// Calcul des forces
	for (int i = 0; i <= N; i++) {
		vcl::vec3 forces = (m + masses_additionnelles[i]) * g - mu * vitesses[i] + forces_exterieures[i];
		for (int j = 1; j <= ordre; j++) {
			if (i + j <= N)
				forces += spring_force(positions[i], positions[i + j], j * L0, K / (float)(j));
			if (i - j >= 0)
				forces += spring_force(positions[i], positions[i - j], j * L0, K / (float)(j));
		}
		liste_forces.push_back(forces);
	}

	// Update des positions
	for (int i = 0; i <= N; i++) {
		if (!(positions_fixes[i])) {
			vitesses[i] = (1 - mu*dt) * vitesses[i] + dt * liste_forces[i] / m;
			mouvMax = std::max(mouvMax, get_norme(dt * vitesses[i]));
			positions[i] = positions[i] + dt * vitesses[i];
		}
	}
	
	return mouvMax;
}


void Corde::repartir() {
	int lastStop = 0;
	int firstNotNull = -1;
	int secondNotNull = -1;
	vcl::vec3 vect;

	for (int i = 0; i <= N; i++) {
		if (positions_fixes[i]) {
			if (firstNotNull < 0)
				firstNotNull = i;
			else if (secondNotNull < 0) {
				secondNotNull = i;

				for (int j = lastStop; j <= secondNotNull; j++) {
					vect = (*references[secondNotNull] - *references[firstNotNull]) / ((float)(secondNotNull - firstNotNull));
					positions[j] = *references[firstNotNull] + (j - firstNotNull) * vect;
				}

				lastStop = i;
				firstNotNull = secondNotNull;
				secondNotNull = -1;
			}
		}
	}

	if (firstNotNull == 0) { // On a trouvé aucun point d'attache
		for (int i = 0; i <= N; i++)
			positions[i] = { 0.0f, i * L0 / (float)(N), 0.0f };
	}
	else if (lastStop == 0) { // On a trouvé un seul point d'attache
		for (int i = 0; i <= N; i++) {
			positions[i] = *references[firstNotNull] +  vcl::vec3(0.0f, (i - firstNotNull) * L0 / (float)(N), 0.0f);
		}
	}
	else if (lastStop < N) { // Il nous manque le dernier morceau de la corde
		for (int j = lastStop + 1; j <= N; j++)
			positions[j] = positions[lastStop] + vect * (j - lastStop);
	}
}


void Corde::equilibre() {
	repartir();

	float dt = 0.01f;
	float epsilon = L0 / 10000.0f;
	float mouvmax = 0;

	do {
		mouvmax = calcul_update(dt);
	} while (mouvmax > epsilon);
}

std::vector<vcl::vec3> Corde::getPositions() {
	return positions;
}

void Corde::libererMorceau(int index) {
	positions_fixes[index] = false;
	positions[index] = { positions[index][0], positions[index][1], positions[index][2] };
}
void Corde::fixerMorceau(int index, vcl::vec3* position) {
	positions_fixes[index] = true;
	references[index] = position;
}



void Corde::ajouterMasse(float u, float masse_, float largeur) {
	for (int i = 0; i <= N; i++)
		masses_additionnelles[i] += masse_*gaussian((float)(i), N * u, largeur);
}

void Corde::resetMasses() {
	for (int i = 0; i <= N; i++)
		masses_additionnelles[i] = 0;
}


float Corde::getCurrentLongueur() {
	float l = 0;
	for (int i = 0; i < N; i++)
		l += get_norme(positions[i + 1] - positions[i]);
	return l;
}


vcl::vec3 Corde::getPos(float u) {
	int i = N * u;
	if(i<N)
		return positions[i] + (N * u - i) * (positions[i + 1] - positions[i]);
	return positions[i];
}

float Corde::getLastLongueur() {
	return get_norme(positions[N] - positions[N - 1]);
}


void Corde::afficher(scene_environment& scene) {
	int n = 8;
	mesh_corde.position.resize((N+1) * n);
	mesh_corde.uv.resize((N+1) * n);

	vcl::vec3 vect1;
	vcl::vec3 vect2;
	float r = 0.1f;

	// Fill terrain geometry
	for (int kv = 0; kv <= N; ++kv)
	{
		// Compute local parametric coordinates (u,v) \in [0,1]
		const float v = kv / ((float)(N));

		vect1 = normalize(produit_vect(positions[kv] - positions[kv + 1], vcl::vec3(1., 1., 1.)));
		vect2 = normalize(produit_vect(positions[kv] - positions[kv + 1], vect1));
		for (int ku = 0; ku < n; ++ku)
		{
			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / (n - 1.0f);

			// Compute the local surface function
			vcl::vec3 const p = positions[kv] +  r * std::cos(2 * vcl::pi * u) * vect1 + r * std::sin(2 * vcl::pi * u) * vect2;
			vcl::vec2 const uv = { u,v };

			// Store vertex coordinates
			mesh_corde.position[kv + N * ku] = p;
			mesh_corde.uv[kv + N * ku] = { u,v };
		}
	}

	// Generate triangle organization
	for (size_t ku = 0; ku < n - 1; ++ku)
	{
		for (size_t kv = 0; kv < N; ++kv)
		{
			const unsigned int idx = kv + N * ku;

			const vcl::uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
			const vcl::uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

			mesh_corde.connectivity.push_back(triangle_1);
			mesh_corde.connectivity.push_back(triangle_2);
		}
	}

	mesh_corde.fill_empty_field();

	vcl::draw(vcl::mesh_drawable(mesh_corde), scene);
}
