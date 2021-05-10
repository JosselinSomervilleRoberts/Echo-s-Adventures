#include "affichable.hpp"

Affichable::Affichable() {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = vcl::vec3(0, 0, 0);
	scale = 1;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = NULL;
}

Affichable::Affichable(vcl::vec3 pos_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = 1;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = NULL;
}

Affichable::Affichable(vcl::vec3 pos_, float scale_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = scale_;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = NULL;
}

Affichable::Affichable(vcl::vec3 pos_, float scale_, bool useTexture_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = scale_;
	rot = vcl::rotation();
	useTexture = useTexture_;
	textureLoader = NULL;
}

Affichable::Affichable(TextureLoader* textureLoader_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = vcl::vec3(0, 0, 0);
	scale = 1;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = textureLoader_;
}

Affichable::Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = 1;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = textureLoader_;
}

Affichable::Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = scale_;
	rot = vcl::rotation();
	useTexture = false;
	textureLoader = textureLoader_;
}

Affichable::Affichable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_, bool useTexture_) {
	translationBase = vcl::vec3(0, 0, 0);
	rotationBase = vcl::rotation();
	scaleBase = 1;
	pos = pos_;
	scale = scale_;
	rot = vcl::rotation();
	useTexture = useTexture_;
	textureLoader = textureLoader_;
}


void Affichable::set_pos(vcl::vec3 pos_)
{ 
	pos = pos_;
	update_posSubElements();
}


void Affichable::set_rotation(vcl::rotation r)
{ 
	rot = r;
	update_posSubElements();
}


void Affichable::set_scale(float s)
{
	scale = s;
	update_posSubElements();
}


void Affichable::set_useTexture(bool use)
{
	useTexture = use;
}


void Affichable::new_subElement() {
	shape.push_back(vcl::mesh());
	relativePos.push_back(vcl::vec3(0, 0, 0));
	visual.push_back(vcl::mesh_drawable());
	textureName.push_back("");
	visualComputed.push_back(false);
	wireFrame.push_back(false);
	displayActivated.push_back(true);
	specular.push_back(1.0f);
}


void Affichable::new_subElements(int nb) {
	for (int i = 0; i < nb; i++)
		new_subElement();
}


void Affichable::update_posSubElements() {
	for (int i = 0; i < shape.size(); i++) {
		if (visualComputed[i]) {
			visual[i].transform.scale = scale * scaleBase;
			visual[i].transform.rotate = rot * rotationBase;
			visual[i].transform.translate = translationBase* scaleBase + pos + scale*scaleBase * visual[i].transform.rotate * relativePos[i];
		}
	}
}


void Affichable::calcul_visual() {
	for (int i = 0; i < shape.size(); i++) {
		if (!(visualComputed[i])) {
			visual[i] = vcl::mesh_drawable(shape[i]);
			if (useTexture && textureLoader != NULL) visual[i].texture = textureLoader->get_textureId(textureName[i]);
			visualComputed[i] = true;
			visual[i].shading.phong.specular = specular[i];
		}
	}
	update_posSubElements();
}


void Affichable::afficher(scene_environment& scene) {
	for (int i = 0; i < shape.size(); i++) {
		if (displayActivated[i] && visualComputed[i]) {
			draw(visual[i], scene);
			if (wireFrame[i]) draw_wireframe(visual[i], scene);
		}
		else if (!(visualComputed[i]))
			std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
	}
}


void Affichable::set_wireFrame(bool use, int i) {
	if (i < wireFrame.size())
		wireFrame[i] = use;
	else
		throw std::invalid_argument("In Affichable  : impossible set_wireFrame of " + std::to_string(i) + " subElement because there are only " + std::to_string(wireFrame.size()) + " subElements (index out of range)");
}



void Affichable::afficher_depth_map(scene_environment& scene) {
	for (int i = 0; i < shape.size(); i++) {
		if (displayActivated[i] && visualComputed[i]) {
			draw_depth_map(visual[i], scene);
			if (wireFrame[i]) draw_wireframe(visual[i], scene);
		}
		else if (!(visualComputed[i]))
			std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
	}
}



void Affichable::afficher_with_shadow(scene_environment& scene) {
	for (int i = 0; i < shape.size(); i++) {
		if (displayActivated[i] && visualComputed[i]) {
			draw_with_shadow(visual[i], scene);
			if (wireFrame[i]) draw_wireframe(visual[i], scene);
		}
		else if (!(visualComputed[i]))
			std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
	}
}