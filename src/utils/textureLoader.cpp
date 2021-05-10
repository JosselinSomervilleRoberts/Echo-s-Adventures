#include "textureLoader.hpp"

TextureLoader::TextureLoader() {
	mainDir = "";
}

TextureLoader::~TextureLoader() {
	/*
    std::map<std::string, ObjModel*>::iterator it = objects.begin();
    while (it != objects.end())
    {
		delete it->second;
        it++;
    }*/
}

TextureLoader::TextureLoader(std::string mainDir_) {
	mainDir = mainDir_;
}

void TextureLoader::load_texture(std::string nameFile) {
	texture[nameFile] = vcl::image_load_png(mainDir + nameFile);
	textureId[nameFile] = vcl::opengl_texture_to_gpu(texture[nameFile], GL_MIRRORED_REPEAT  /**GL_TEXTURE_WRAP_S*/, GL_MIRRORED_REPEAT  /**GL_TEXTURE_WRAP_T*/);
}

GLuint TextureLoader::get_textureId(std::string nameFile) {
	if (textureId.find(nameFile) == textureId.end())
		load_texture(nameFile);
	return textureId[nameFile];
}

/*
ObjModel* TextureLoader::get_model(std::string nameModel) {
	if (objects.find(nameModel) == objects.end())
		load_model(nameModel);
	return objects[nameModel];
}

void TextureLoader::load_model(std::string nameModel) {
	ObjModel* obj = new ObjModel(nameModel, this);
	objects[nameModel] = obj;
}*/