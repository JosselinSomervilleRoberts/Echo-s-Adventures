#pragma once

#include "affichable.hpp"
#include <iostream>
#include <list>

class Box : public Affichable {

	protected:
		vcl::vec3 vect1;
		vcl::vec3 vect2;
		vcl::vec3 vect3;

	public:
		Box(vcl::vec3 vect1_, vcl::vec3 vect2_, vcl::vec3 vect3_);
		Box(vcl::vec3 vect1_, vcl::vec3 vect2_, vcl::vec3 vect3_, TextureLoader* texture_loader_, std::string textureName_);

		void calcul_shape();
};