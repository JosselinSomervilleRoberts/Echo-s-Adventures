#include "objModel.hpp"


ObjModel::ObjModel() : Affichable() {
	modelName = "";
	folderName = "assets/";
}

ObjModel::ObjModel(std::string modelName_, TextureLoader* textureLoader_) : Affichable(textureLoader_) {
	modelName = modelName_;
	folderName = "assets/";
	calcul_shape();
	calcul_visual();
}

void ObjModel::calcul_shape() {
	// Parametres de l'affichable
	useTexture = true;

	std::vector<std::string> lines = read_lines(folderName + modelName + "/infos.txt");
	bool inSubModel = false;
	bool loadedSubModel = true;

	for(int i=0; i<lines.size(); i++) {
		std::string line = lines[i];

		if (line.find("SUBMODEL") != std::string::npos) {// Nouveau submodele
			if (!(loadedSubModel)) throw std::invalid_argument("In ObjModel " + modelName + " : impossible to load new subModel if previous one wasn't fully loaded (name missing)");
			new_subElement();
			loadedSubModel = false;
			inSubModel = true;
		}
		else if (line.find("MODEL") != std::string::npos) {// Pour la définition des paramètres généraux
			inSubModel = false;
		}
		else if (inSubModel) {

			if (line.find("name") != std::string::npos) {
				std::vector<std::string> toBeErased{ " ", "=", "-", "name" };
				eraseSubStrings(line, toBeErased);
				shape.back() = vcl::mesh_load_file_obj(folderName + modelName + "/model/" + line + ".obj");
				textureName.back() = folderName + modelName + "/textures/" + line + ".png";
				loadedSubModel = true;
			}
			else if (line.find("specular") != std::string::npos) {
				std::vector<std::string> toBeErased{ " ", "=", "-", "specular" };
				eraseSubStrings(line, toBeErased);
				specular.back() = std::stof(line);
			}
		}
		else {
			if (line.find("scale") != std::string::npos) {
				std::vector<std::string> toBeErased{ " ", "=", "-", "scale" };
				eraseSubStrings(line, toBeErased);
				scaleBase = std::stof(line);
			}
			if (line.find("rotate") != std::string::npos) {
				std::vector<std::string> toBeErased{ " ", "=", "-", "rotate" };
				eraseSubStrings(line, toBeErased);
				std::vector<std::string> coords = split(line, ",");
				if (coords.size() != 3) throw std::invalid_argument("In ObjModel " + modelName + " : rotation of subModel has not 3 coordinates (coords incorrect)");
				rotationBase = vcl::rotation(vcl::vec3(0, 0, 1), (vcl::pi/180.0f)*std::stof(coords[2])) * vcl::rotation(vcl::vec3(0, 1, 0), (vcl::pi / 180.0f) * std::stof(coords[1])) * vcl::rotation(vcl::vec3(1, 0, 0), (vcl::pi / 180.0f) * std::stof(coords[0]));
			}
			if (line.find("translate") != std::string::npos) {
				std::vector<std::string> toBeErased{ " ", "=", "-", "translate" };
				eraseSubStrings(line, toBeErased);
				std::vector<std::string> coords = split(line, ",");
				if (coords.size() != 3) throw std::invalid_argument("In ObjModel " + modelName + " : translation of subModel has not 3 coordinates (coords incorrect)");
				translationBase = vcl::vec3(std::stof(coords[0]), std::stof(coords[1]), std::stof(coords[2]));
			}
		}
	}
	if (!(loadedSubModel)) throw std::invalid_argument("In ObjModel " + modelName + " : loading of model has ended but last subModel wasn't fully loaded (name missing)");
}