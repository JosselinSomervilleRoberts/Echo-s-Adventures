#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "textureLoader.hpp"
#include "utils.hpp"
#include "shadow_map.hpp"

#include <iostream>
#include  <list>


class Affichable {

	protected:
		vcl::vec3 pos;
		float scale;
		vcl::rotation rot;
		bool useTexture;
		vcl::vec3 translationBase;
		vcl::rotation rotationBase;
		float scaleBase;

		std::vector<vcl::mesh> shape;
		std::vector<vcl::vec3> relativePos;
		std::vector<vcl::mesh_drawable> visual;
		std::vector<std::string> textureName;
		std::vector<bool> visualComputed;
		std::vector<bool> displayActivated;
		std::vector<bool> wireFrame;
		std::vector<float> specular;

		TextureLoader* textureLoader;

	public:
		Affichable();
		Affichable(vcl::vec3 pos_);
		Affichable(vcl::vec3 pos_, float scale_);
		Affichable(vcl::vec3 pos_, float scale_, bool useTexture_);
		Affichable(TextureLoader* textureLoader_);
		Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_);
		Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_);
		Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_, bool useTexture_);

		void set_pos(vcl::vec3 pos_);
		void set_rotation(vcl::rotation r);
		void set_scale(float s);
		void set_useTexture(bool use);
		void set_wireFrame(bool use, int i);
		vcl::vec3 get_pos() { return pos; };
		vcl::rotation get_rotation() { return rot; };
		double get_scale() { return scale; };
		bool get_useTexture() { return useTexture; };

		void new_subElement();
		void new_subElements(int nb);
		void update_posSubElements();

		virtual void calcul_visual();
		virtual void afficher(scene_environment& scene);
		virtual void afficher_depth_map(scene_environment& scene);
		virtual void afficher_with_shadow(scene_environment& scene);
		virtual void calcul_shape() = 0;
};