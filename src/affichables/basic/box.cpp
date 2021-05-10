#include "box.hpp"
#include "utils.hpp"
using namespace vcl;


Box::Box(vcl::vec3 vect1_, vcl::vec3 vect2_, vcl::vec3 vect3_) : Affichable() {
	vect1 = vect1_;
	vect2 = vect2_;
	vect3 = vect3_;
	calcul_shape();
	calcul_visual();
}


Box::Box(vcl::vec3 vect1_, vcl::vec3 vect2_, vcl::vec3 vect3_, TextureLoader* texture_loader_, std::string textureName_) : Affichable(texture_loader_)  {
	vect1 = vect1_;
	vect2 = vect2_;
	vect3 = vect3_;
	calcul_shape();
	textureName[0] = textureName_;
	useTexture = true;
	calcul_visual();
}



void Box::calcul_shape()
{
	if (shape.size() == 0) new_subElement();

	vec3 p000 = vect1 + vect2 + vect3;
	vec3 p100 = - vect1 + vect2 + vect3;
	vec3 p110 = - vect1 - vect2 + vect3;
	vec3 p010 = vect1 - vect2 + vect3;
	vec3 p001 = vect1 + vect2 - vect3;
	vec3 p101 = - vect1 + vect2 - vect3;
	vec3 p111 = - vect1 - vect2 - vect3;
	vec3 p011 = vect1 - vect2 - vect3;

	vcl::mesh quad= mesh_primitive_quadrangle(p000, p100, p101, p001);
	quad.uv = { {0,0}, {0,1}, {1,0}, {1,1} };
	shape[0].push_back(quad);
	shape[0].push_back(mesh_primitive_quadrangle(p100, p110, p111, p101));
	shape[0].push_back(mesh_primitive_quadrangle(p110, p010, p011, p111));
	shape[0].push_back(mesh_primitive_quadrangle(p010, p000, p001, p011));
	shape[0].push_back(mesh_primitive_quadrangle(p001, p101, p111, p011));
	shape[0].push_back(mesh_primitive_quadrangle(p100, p000, p010, p110));
}