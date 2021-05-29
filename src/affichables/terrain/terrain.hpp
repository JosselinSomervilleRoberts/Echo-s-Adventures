#pragma once

#include "vcl/vcl.hpp"
#include "utils.hpp"
#include "affichable.hpp"
#include "perlin_noise_parameters.hpp"
#include "chunk.hpp"
#include "../models/objModel.hpp"

#include <algorithm>
#include <list>
#include <iostream>
#include <map>


struct chunkPos {
	chunkPos() { x = 0; y = 0; };
	chunkPos(int x_, int y_) { x = x_; y = y_; };
	//chunkPos(float x_, float y_) { x = x_; y = y_; };
	int x = 0;
	int y = 0;


	bool operator<(const chunkPos& a) const  {
		return (x < a.x || (a.x == x && y < a.y));
	}
	
	bool operator==(const chunkPos& rhs) const
	{
		return ((rhs.x == x) && (rhs.y == y));
	}
};


class Terrain : public Affichable {

	protected:
		float chunkSize;
		size_t N;
		std::map<chunkPos, Chunk> chunks;
		std::map<std::string, ObjModel*> objects;
		float distMaxRender = 20.0;
		std::vector<chunkPos> toBeLoaded;


	public:
		bool eyesActivated = false;
		vcl::vec3 posLoup;

		Terrain();
		Terrain(float chunkSize_, size_t N_);
		Terrain(float chunkSize_, size_t N_, TextureLoader* textureLoader_);
		void newChunk(int x, int y);
		void calcul_shape();
		void afficher(scene_environment& scene);
		void afficher(scene_environment& scene, float distMin, float distMax);
		void afficher_depth_map(scene_environment& scene);
		void afficher_with_shadow(scene_environment& scene);
		void load_model(std::string modelName);
		void load_models();

		float get_heightOfTerrain(float x, float y);
		std::vector<vcl::vec3> get_posOnTerrain(float x, float y);
		void initiateChunks();

		vcl::vec3 get_nearestPierre(vcl::vec3 pos_);
		bool has_pierre(vcl::vec3 pos_);

		void update(float dt);
};


//std::vector<vcl::vec3> generate_positions_on_terrain(int N);
//std::vector<vcl::vec3> generate_positions_on_terrain(int N, double rayon);
//std::vector<vcl::vec3> generate_positions_on_terrain(vcl::mesh& terrain, int N, double marge, std::vector<int>& types, std::vector<double>& rayons);
double distance_plane(vcl::vec3 pos1, vcl::vec3 pos2);
vcl::vec3 get_pos(vcl::mesh& terrain, float u, float v);
//std::vector<vcl::vec2> poissonDiscSampler(float sx, float sy, float ex, float ey, float r, int k);

