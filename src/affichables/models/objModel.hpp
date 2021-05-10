#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include "textureLoader.hpp"
#include "affichable.hpp"
#include "utils.hpp"

#include <iostream>
#include  <list>


class ObjModel : public Affichable {

protected:
	std::string modelName;
	std::string folderName;


public:
	ObjModel();
	ObjModel(std::string modelName_, TextureLoader* textureLoader_);
	void calcul_shape();
};