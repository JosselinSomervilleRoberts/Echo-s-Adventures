#pragma once

#include "vcl/vcl.hpp"
#include "utils.hpp"
#include "Affichable.hpp"
#include "perlin_noise_parameters.hpp"
#include "interpolation.hpp"
#include "side.hpp"
#include "../models/objModel.hpp"

#include <algorithm>
#include <list>
#include <iostream>


class Chunk : public Affichable {

protected:
	int xChunk, yChunk;
	float chunkSize;
	size_t N;
	std::vector<Side> sides;
	std::vector<Side> sidesContraints;
	perlin_noise_parameters parameters;

	std::vector<float> hMontagne;
	std::vector<float> sigmaMontagne;
	std::vector <vcl::vec2> posMontagne;


	vcl::vec3 evaluate_terrain(float u, float v);
	std::vector<ObjModel*> arbres;
	std::vector<vcl::vec3> posObjets;
	std::vector<vcl::vec3> nObjets;
	std::vector<vcl::rotation> rotObjets;
	std::vector<int> indexObjets;

public:
	Chunk();
	Chunk(int x_, int y_, size_t N_, float chunk_size_);
	Chunk(int x_, int y_, size_t N_, float chunk_size_, TextureLoader* textureLoader_);
	Chunk(int x_, int y_, size_t N_, float chunk_size_, TextureLoader* textureLoader_, std::vector<ObjModel*> arbres_);

	std::vector<vcl::vec3> get_posOnTerrain(vcl::vec2 pos2D);
	std::vector<vcl::vec3> get_posOnTerrain(vcl::vec3 pos);
	std::vector<vcl::vec3> get_posOnTerrain(float x, float y);
	
	void set_side(int index, Side side_);
	Side get_side(int index) { return sides[index]; };
	void calcul_chunk();
	void initiate_sides();
	void correct_chunk();
	void correct_chunk_der();
	void add_perlinNoise();
	void calcul_normals();
	void finish_creation();
	void calcul_shape();
	void calcul_side();
	void calcul_randomterrain();

	void add_arbres();
	void afficher(scene_environment& scene);
	void afficher_depth_map(scene_environment& scene);
	void afficher_with_shadow(scene_environment& scene);

	void ajouterMontagne(vcl::vec2 centre, float hautMontagne, float largeur);
};

