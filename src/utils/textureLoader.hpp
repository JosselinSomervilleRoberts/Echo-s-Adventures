#pragma once
#include "vcl/vcl.hpp"
#include "utils.hpp"
//#include "objModel.hpp"
#include <iostream>
#include <map>


//class ObjModel;

class TextureLoader {

	protected:
		std::string mainDir;
		std::map<std::string, vcl::image_raw> texture;
		std::map<std::string, GLuint> textureId;
		//std::map<std::string, ObjModel*> objects;


	public:
		TextureLoader();
		~TextureLoader();
		TextureLoader(std::string mainDir_);
		void load_texture(std::string nameFile);
		GLuint get_textureId(std::string nameFile);
		//void load_model(std::string modelName);
		//ObjModel* get_model(std::string nameModel);
};