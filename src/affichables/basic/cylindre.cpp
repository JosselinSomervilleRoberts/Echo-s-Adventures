#include "cylindre.hpp"
#include "utils.hpp"
using namespace vcl;


Cylindre::Cylindre(double r_, double h_) : Affichable() {
    r = r_;
    h = h_;
    N = 20;
    noise = 0;
    normal = vcl::vec3(0, 0, 1);

    calcul_shape();
    useTexture = false;
    calcul_visual();
}


Cylindre::Cylindre(double r_, double h_, double noise_) : Affichable() {
    r = r_;
    h = h_;
    N = 20;
    noise = noise_;
    normal = vcl::vec3(0, 0, 1);

    calcul_shape();
    useTexture = false;
    calcul_visual();
}


Cylindre::Cylindre(double r_, double h_, double noise_, size_t N_) : Affichable() {
    r = r_;
    h = h_;
    N = N_;
    noise = noise_;
    normal = vcl::vec3(0, 0, 1);

    calcul_shape();
    useTexture = false;
    calcul_visual();
}


Cylindre::Cylindre(double r_, double h_, double noise_, TextureLoader* textureLoader_,  std::string name_texture_cylindre, std::string name_texture_disque) : Affichable(textureLoader_) {
    r = r_;
    h = h_;
    N = 20;
    noise = noise_;
    normal = vcl::vec3(0, 0, 1);

    calcul_shape();
    useTexture = true;
    textureName[0] = name_texture_cylindre;
    textureName[1] = name_texture_disque;
    calcul_visual();
}


Cylindre::Cylindre(double r_, double h_, double noise_, size_t N_, TextureLoader* textureLoader_,  std::string name_texture_cylindre, std::string name_texture_disque) : Affichable(textureLoader_) {
    r = r_;
    h = h_;
    N = N_;
    noise = noise_;
    normal = vcl::vec3(0, 0, 1);

    calcul_shape();
    useTexture = true;
    textureName[0] = name_texture_cylindre;
    textureName[1] = name_texture_disque;
    calcul_visual();
}


void Cylindre::calcul_shape() {
    calcul_cylindre();
    calcul_disque();
}


void Cylindre::calcul_cylindre()
{
    if (shape.size() == 0) new_subElement();

    shape[0].position.resize(N * N);
    shape[0].uv.resize(N * N);

    // Fill terrain geometry
    for (unsigned int ku = 0; ku < N; ++ku)
    {
        for (unsigned int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            // Compute the local surface function
            double dr = r*noise*(0.5-(float)rand() / RAND_MAX);
            vcl::vec3 const p = { (r+dr)* std::cos(2 * vcl::pi * u), (r+dr) * std::sin(2 * vcl::pi * u), h * (v - 0.5f) };
            vcl::vec2 const uv = { u,v };

            // Store vertex coordinates
            shape[0].position[kv + N * ku] = p;
            shape[0].uv[kv + N * ku] = { u,v };
        }
    }

    // Generate triangle organization
    for (size_t ku = 0; ku < N - 1; ++ku)
    {
        for (size_t kv = 0; kv < N - 1; ++kv)
        {
            const unsigned int idx = kv + N * ku;

            const vcl::uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            const vcl::uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            shape[0].connectivity.push_back(triangle_1);
            shape[0].connectivity.push_back(triangle_2);
        }
    }

    shape[0].fill_empty_field();
}


void Cylindre::calcul_disque()
{
    if (shape.size() < 2) new_subElements(2 - shape.size());

    for (size_t k = 0; k < size_t(N); ++k)
    {
        float const u = k / (N - 1.0f);
        vcl::vec3 const p = r * vcl::vec3(std::cos(2 * vcl::pi * u), std::sin(2 * vcl::pi * u), 0.0f);
        shape[1].position.push_back(p);
        shape[1].uv.push_back({ 0.5 + 0.4 * std::cos(2 * vcl::pi * u), 0.5 + 0.4 * std::sin(2 * vcl::pi * u) });

    }
    // middle point
    shape[1].position.push_back({ 0,0,0 });
    shape[1].uv.push_back({ 0.5,0.5 });

    for (size_t k = 0; k < size_t(N - 1); ++k)
        shape[1].connectivity.push_back(vcl::uint3{ unsigned(N), unsigned(k), unsigned(k + 1) });

    shape[1].fill_empty_field();
}


void Cylindre::set_display_disque(bool b) {
    if (displayActivated.size() == 2) displayActivated[1] = b;
}


void Cylindre::afficher(scene_environment& scene) {
    visual[0].transform.translate = pos + scale * h / 2.0 * rot * normal;
    draw(visual[0], scene);
    if (displayActivated[1]) {
        visual[1].transform.translate = pos;
        draw(visual[1], scene);
        visual[1].transform.translate = pos + scale * h * rot * normal;
        draw(visual[1], scene);
    }
}
