#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "textureLoader.hpp"
#include "affichable.hpp"
#include "utils.hpp"

#include <iostream>
#include  <list>


class AffichableUpdatable : public Affichable {

	protected:
		std::vector<vcl::vec3 *> references;
		std::vector<vcl::vec3> lastPosReferences;

		float lastMouv;
		float threshold;
		bool needsToBeUpdated;


	public:
		AffichableUpdatable();
		AffichableUpdatable(vcl::vec3 pos_);
		AffichableUpdatable(vcl::vec3 pos_, float scale_);
		AffichableUpdatable(vcl::vec3 pos_, float scale_, bool useTexture_);
		AffichableUpdatable(TextureLoader* textureLoader_);
		AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_);
		AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_);
		AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_, bool useTexture_);

		virtual void calcul_shape() = 0;
		virtual float calcul_update(float dt) = 0;
		void update(float dt);
		void add_reference(vcl::vec3* ref);
};