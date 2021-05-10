#include "affichableUpdatable.hpp"

AffichableUpdatable::AffichableUpdatable() : Affichable() {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(vcl::vec3 pos_) : Affichable(pos_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(vcl::vec3 pos_, float scale_) : Affichable(pos_, scale_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(vcl::vec3 pos_, float scale_, bool useTexture_) : Affichable(pos_, scale_, useTexture_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(TextureLoader* textureLoader_) : Affichable(textureLoader_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_) : Affichable(textureLoader_, pos_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_) : Affichable(textureLoader_, pos_, scale_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


AffichableUpdatable::AffichableUpdatable(TextureLoader* textureLoader_, vcl::vec3 pos_, float scale_, bool useTexture_) : Affichable(textureLoader_, pos_, scale_, useTexture_) {
	needsToBeUpdated = true;
	threshold = 0.05f;
	lastMouv = 0.0f;
}


void AffichableUpdatable::update(float dt) {
	float mouvReferences = 0;
	for (int i = 0; i < references.size(); i++)
		mouvReferences = std::max(mouvReferences, get_norme(lastPosReferences[i] - *(references[i])));
		
	needsToBeUpdated = (needsToBeUpdated || (mouvReferences >= threshold));
	if (needsToBeUpdated) {
		for (int i = 0; i < references.size(); i++)
			lastPosReferences[i] = *(references[i]);

		lastMouv = calcul_update(dt);
		needsToBeUpdated = (lastMouv >= threshold);
	}
}



void AffichableUpdatable::add_reference(vcl::vec3* ref) {
	references.push_back(ref);
	lastPosReferences.push_back(*ref);
}