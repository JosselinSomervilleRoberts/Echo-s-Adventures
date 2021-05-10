#include "chunk.hpp"

Chunk::Chunk() : Affichable() {
    xChunk = 0;
    yChunk = 0;
    chunkSize = 10;
    N = 200;
    //arbre = NULL;
    initiate_sides();
}

Chunk::Chunk(int x_, int y_, size_t N_, float chunkSize_) : Affichable() {
    xChunk = x_;
    yChunk = y_;
    chunkSize = chunkSize_;
    //pos = chunkSize * vcl::vec3(x_, y_, 0);
	N = N_;
    //arbre = NULL;
    //initiate_sides();
    initiate_sides();
    calcul_shape();
    //set_wireFrame(true, 0);
}


Chunk::Chunk(int x_, int y_, size_t N_, float chunkSize_, TextureLoader* textureLoader_) : Affichable(textureLoader_) {
    xChunk = x_;
    yChunk = y_;
    chunkSize = chunkSize_;
    //pos = chunkSize * vcl::vec3(x_, y_, 0);
    N = N_;
    //arbre = NULL;
    //initiate_sides();
    initiate_sides();
    calcul_shape();
    //set_wireFrame(true, 0);
}

Chunk::Chunk(int x_, int y_, size_t N_, float chunkSize_, TextureLoader* textureLoader_, std::vector<ObjModel*> arbres_) : Affichable(textureLoader_) {
    xChunk = x_;
    yChunk = y_;
    chunkSize = chunkSize_;
    //pos = chunkSize * vcl::vec3(x_, y_, 0);
    N = N_;
    arbres =  arbres_;
    //initiate_sides();

    ajouterMontagne(vcl::vec2(-11, 3), 2, 8);
    ajouterMontagne(vcl::vec2(14, 4.5), 1.8, 7);
    ajouterMontagne(vcl::vec2(6, -5), 0.2, 7);
    
    //std::cout << std::endl;


    initiate_sides();
    calcul_shape();
    add_arbres();
    //set_wireFrame(true, 0);
}


void Chunk::ajouterMontagne(vcl::vec2 centre, float hautMontagne, float largeur) {
    hMontagne.push_back(12);
    posMontagne.push_back(centre);
    sigmaMontagne.push_back(largeur);

    for (int i = 0; i < 5 * (1 + vcl::noise_perlin(centre, 3, 0.8, 0.8)); i++) {
        float angle = 5 * vcl::pi * (vcl::noise_perlin((float)(i)+vcl::noise_perlin(centre[0] + centre[1])) - 0.5);
        //std::cout << " angle= " << angle << std::endl;
        float r = 2 * (largeur)*vcl::noise_perlin(angle / (2 * vcl::pi));
        posMontagne.push_back(centre + r * vcl::vec2(std::cos(angle), std::sin(angle)));
        sigmaMontagne.push_back(largeur * 0.8 * (0.2 + vcl::noise_perlin(r * angle / (2 * vcl::pi))));
        hMontagne.push_back(hautMontagne * 0.6 * (-1.1f + vcl::noise_perlin(sigmaMontagne[sigmaMontagne.size() - 1] / 5.0f)));
    }
}

std::vector<vcl::vec3> Chunk::get_posOnTerrain(vcl::vec3 pos) {
    return get_posOnTerrain(vcl::vec2(pos[0], pos[1]));
}

std::vector<vcl::vec3> Chunk::get_posOnTerrain(float x, float y) {
    return get_posOnTerrain(vcl::vec2(x,y));
}

std::vector<vcl::vec3> Chunk::get_posOnTerrain(vcl::vec2 pos2D) {
    float const u = ((pos2D[0] / (chunkSize)) - xChunk) / 1.0f + 0.5;
    float const v = ((pos2D[1] / (chunkSize)) - yChunk) / 1.0f + 0.5;
    int const ku = (float)(N - 1) * u;
    int const kv = (float)(N - 1) * v;

    // Le carré est découpé en 2 triangles :
    //  (ku,kv) -------------- (ku+1,kv)
    //     |                    /  |
    //     |                 /     |
    //     |      1       /        |
    //     |           /           |
    //     |        /              |
    //     |     /         2       |
    //     |  /                    |
    // (ku,kv+1) ----------- (ku+1,kv+1)

    // Il faut déterminer dans lequel des deux triangles le point est
    float d1 = norm(pos2D - vcl::vec2(shape[0].position[kv + N * ku][0], shape[0].position[kv + N * ku][1]));
    float d2 = norm(pos2D - vcl::vec2(shape[0].position[kv+1 + N * (ku+1)][0], shape[0].position[kv+1 + N * (ku+1)][1]));

    vcl::vec3 v1, v2;
    if (d1 <= d2) { // On est dans le triangle 1
        v1 = shape[0].position[kv+1 + N * ku] - shape[0].position[kv + N * ku];
        v2 = shape[0].position[kv + N * (ku + 1)] - shape[0].position[kv + N * ku];
    }
    else { // On est dans le triangle 2
        v1 = shape[0].position[kv + 1 + N * ku] - shape[0].position[kv + 1 + N * (ku + 1)];
        v2 = shape[0].position[kv + N * (ku+1)] - shape[0].position[kv + 1 + N * (ku + 1)];
    }

    vcl::vec3  n = produit_vect(v1, v2);
    if (produit_scal(n, vcl::vec3(0,0,1)) < 0) n *= -1; // On s'assure que le vecteur soit ascendant
    n = normalize(n);

    vcl::vec3 const posPlan = shape[0].position[kv + 1 + N * ku];
    vcl::vec3 new_pos = vcl::vec3(pos2D[0], pos2D[1], 0) - produit_scal(vcl::vec3(0,0,1), vcl::vec3(pos2D[0], pos2D[1], 0) - posPlan) * vcl::vec3(0, 0, 1);

    std::vector<vcl::vec3> result;
    result.push_back(new_pos);
    result.push_back(n);
    return result;
}



vcl::vec3 Chunk::evaluate_terrain(float u, float v) {
    float const x = chunkSize * (xChunk + (u - 0.5f));
    float const y = chunkSize * (yChunk + (v - 0.5f));

    float z = 0;
    for (int i = 0; i < posMontagne.size(); i++) {
        float const d = vcl::norm(vcl::vec2(x, y) - posMontagne[i]) / sigmaMontagne[i];
        z += hMontagne[i] * std::exp(-d * d);
    }

    return vcl::vec3(x,y,z);
}


void Chunk::calcul_chunk() {
    //vcl::mesh& terrain = shape[0];

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
            vcl::vec3 const p = evaluate_terrain(u, v);

            // Store vertex coordinates
            shape[0].position[kv + N * ku] = p;
            shape[0].uv[kv + N * ku] = { 5 * u,5 * v };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (size_t ku = 0; ku < N - 1; ++ku)
    {
        for (size_t kv = 0; kv < N - 1; ++kv)
        {
            const unsigned int idx = kv + N * ku; // current vertex offset

            const vcl::uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            const vcl::uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            shape[0].connectivity.push_back(triangle_1);
            shape[0].connectivity.push_back(triangle_2);
        }
    }

    shape[0].fill_empty_field(); // need to call this function to fill the other buffer with default values (normal, color, etc)
}


void Chunk::correct_chunk_der() {
    /*
    *  (0,N-1) side3 (N-1,N-1)
    *    s               s
    *    i               i
    *    d               d
    *    e               e
    *    4               2
    *  (0,0)   side1  (N-1,0)
    */
    vcl::mesh& terrain = shape[0];

    calcul_side();
    for (int i = 0; i < 4; i++)
        sides[i].constrained = false;

    std::vector<Side> decalages;
    for (int i = 0; i < 4; i++) {
        if (!(sidesContraints[i].constrained)) {
            float pos0 = 0;
            float der0 = 0;
            float pos1 = 0;
            float der1 = 0;

            int index = (i - 1) % 4;
            if (sidesContraints[index].constrained) {
                if (index < 0) index += 4;
                der0 = sidesContraints[index].derivatives[sides[index].derivatives.size() - 1] - sides[i].derivatives[0];// -der0;
            }

            if (sidesContraints[(i + 1) % 4].constrained) {
                der1 = sidesContraints[(i + 1) % 4].derivatives[0] - sides[i].derivatives[sides[i].derivatives.size() - 1];// -der1;
            }

            //std::cout << "generate side " << i << " (x=" << xChunk << ", y=" << yChunk << ")" << std::endl;
            Side decal;
            decal.generate_side(N, pos0, der0, pos1, der1);
            decalages.push_back(decal);
        }
        else {
            //std::cout << "========= side already ok" << std::endl;
            Side decal;
            decal = sidesContraints[i] - sides[i];
            decalages.push_back(decal);
        }

        for (int j = 0; j< 20; j++) {
            //std::cout << "axe " << i << " : " << decalages[i].positions[j] << " / der = " << decalages[i].derivatives[j] << std::endl;
        }
    }


    float z_interpol;
    for (int kx = 0; kx < N; kx++) {
        for (int ky = 0; ky < N; ky++) {
            //float z_interpol_x = interpolation(kx / (float)(N - 1), decalages[3].positions[N - 1 - ky], decalages[1].positions[ky]);
            //float z_interpol_y = interpolation(ky / (float)(N - 1), decalages[0].positions[kx], decalages[2].positions[N - 1 - kx]);
            float z_interpol_x = interpolation2(kx / (float)(N - 1), decalages[3].positions[N - 1 - ky], decalages[3].derivatives[N - 1 - ky], decalages[1].positions[ky], decalages[1].derivatives[ky]);
            float z_interpol_y = interpolation2(ky / (float)(N - 1), decalages[0].positions[kx], decalages[0].derivatives[kx], decalages[2].positions[N - 1 - kx], decalages[2].derivatives[N - 1 - kx]);
            z_interpol = z_interpol_x;
            /*
            if ((kx == 0) || (kx == N - 1))
                z_interpol = z_interpol_x;
            else if ((ky == 0) || (ky == N - 1))
                z_interpol = z_interpol_y;
            else {
                float coef_x = 1.0f / pow((std::min((float)(kx), (float)(N - 1 - kx))), 1);
                float coef_y = 1.0f / pow((std::min((float)(ky), (float)(N - 1 - ky))), 1);
                z_interpol = (coef_x * z_interpol_x + coef_y * z_interpol_y) / (coef_x + coef_y);
            }*/
            terrain.position[ky + N * kx][2] += z_interpol;
        }
    }
}



void Chunk::correct_chunk() {
    /*
    *  (0,N-1) side3 (N-1,N-1)
    *    s               s
    *    i               i
    *    d               d
    *    e               e
    *    4               2
    *  (0,0)   side1  (N-1,0)
    */
    vcl::mesh& terrain = shape[0];

    calcul_side();
    for (int i = 0; i < 4; i++)
        sides[i].constrained = false;

    std::vector<Side> decalages;
    for (int i = 0; i < 4; i++) {
        if (!(sidesContraints[i].constrained)) {
            float pos0 = 0;
            float der0 = 0;
            float pos1 = 0;
            float der1 = 0;

            int index = (i - 1) % 4;
            if (sidesContraints[index].constrained) {
                if (index < 0) index += 4;
                pos0 = sidesContraints[index].positions[sides[index].positions.size() - 1] - sides[i].positions[0];// -pos0;
                der0 = sidesContraints[index].derivatives[sides[index].derivatives.size() - 1] - sides[i].derivatives[0];// -der0;
            }

            if (sidesContraints[(i + 1) % 4].constrained) {
                pos1 = sidesContraints[(i + 1) % 4].positions[0] - sides[i].positions[sides[i].positions.size() - 1];// -pos1;
                der1 = sidesContraints[(i + 1) % 4].derivatives[0] - sides[i].derivatives[sides[i].derivatives.size() - 1];// -der1;
            }

            //std::cout << "generate side " << i << " (x=" << xChunk << ", y=" << yChunk << ")" << std::endl;
            Side decal;
            decal.generate_side(N, pos0, der0, pos1, der1);
            decalages.push_back(decal);
        }
        else {
            //std::cout << "========= side already ok" << std::endl;
            Side decal;
            decal = sidesContraints[i] - sides[i];
            decalages.push_back(decal);
        }

        for (int j = 0; j < 20; j++) {
            //std::cout << "axe " << i << " : " << decalages[i].positions[j] << " / der = " << decalages[i].derivatives[j] << std::endl;
        }
    }


    float z_interpol;
    for (int kx = 0; kx < N; kx++) {
        for (int ky = 0; ky < N; ky++) {
            //float z_interpol_x = interpolation(kx / (float)(N - 1), decalages[3].positions[N - 1 - ky], decalages[1].positions[ky]);
           // float z_interpol_y = interpolation(ky / (float)(N - 1), decalages[0].positions[kx], decalages[2].positions[N - 1 - kx]);
            float z_interpol_x = interpolation2(kx / (float)(N - 1), decalages[3].positions[N - 1 - ky], decalages[3].derivatives[N - 1 - ky], decalages[1].positions[ky], decalages[1].derivatives[ky]);
            float z_interpol_y = interpolation2(ky / (float)(N - 1), decalages[0].positions[kx], decalages[0].derivatives[kx], decalages[2].positions[N - 1 - kx], decalages[2].derivatives[N - 1 - kx]);

            if ((kx == 0) || (kx == N - 1))
                z_interpol = z_interpol_x;
            else if ((ky == 0) || (ky == N - 1))
                z_interpol = z_interpol_y;
            else {
                float coef_x = 1.0f / pow((std::min((float)(kx), (float)(N - 1 - kx))), 1);
                float coef_y = 1.0f / pow((std::min((float)(ky), (float)(N - 1 - ky))), 1);
                z_interpol = (coef_x * z_interpol_x + coef_y * z_interpol_y) / (coef_x + coef_y);
            }
            terrain.position[ky + N * kx][2] += z_interpol;
        }
    }
}


void Chunk::initiate_sides() {
    for (int j = 0; j < 4; j++) {
        Side side(N);
        sides.push_back(side);
        Side side2(N);
        sidesContraints.push_back(side2);
    }
}


void Chunk::add_perlinNoise()
{
    //vcl::mesh& terrain = shape[0];

    // Recompute the new vertices
    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {

            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            int const idx = kv * N + ku;

            // Compute the Perlin noise
            float const noise = vcl::noise_perlin({ 0.1*(u+yChunk),0.1*(v+xChunk) }, parameters.octave, parameters.persistency, parameters.frequency_gain);

            // use the noise as height value
            shape[0].position[idx].z += parameters.terrain_height * noise;

            // use also the noise as color value
            shape[0].color[idx] = 0.3f * vcl::vec3(0, 0.5f, 0) + 0.7f * noise * vcl::vec3(1, 1, 1);
        }
    }
}

void Chunk::calcul_normals() {
    // Update the normal of the mesh structure
    shape[0].compute_normal();
}


void Chunk::calcul_shape() {
    if (shape.size() < 1) new_subElement();
    calcul_randomterrain();
    calcul_chunk();
    add_perlinNoise();
}

void Chunk::set_side(int index, Side side_) {
    if ((index < 4) && (index >= 0) && (side_.positions.size() == N) && (sidesContraints.size() >= 4)) {
        //std::cout << "ok set_side" << std::endl;
        for (int i = 0; i < N; i++) {
            sidesContraints[index].positions[i] = side_.positions[i];
            sidesContraints[index].derivatives[i] = side_.derivatives[i];
        }
        sidesContraints[index].constrained = true;
    }
    else {
        //std::cout << "not ok N=" << N << " et size=" << side_.positions.size() << std::endl;
    }
}

void Chunk::finish_creation() {
    //correct_chunk();
    //correct_chunk_der();
    calcul_side();
    calcul_normals();
    specular[0] = 0;
    textureName[0] = "images/texture_grass.png";
    useTexture = true;
    calcul_visual();
}

void Chunk::calcul_side() {
    int kx, ky;
    float p;
    float d;
    int decalage = 1;
    for (int i = 0; i < N; i++) {
        p = shape[0].position[0 + N * (i)][2];
        d = -(p - shape[0].position[decalage + N * (i)][2]) / (decalage*chunkSize / (float)(N-1));
        sides[0].positions[i] = p;
        sides[0].derivatives[i] = d;


        p = shape[0].position[(i) + N * (N - 1)][2];
        d = (p - shape[0].position[(i) + N * (N - 1 - decalage)][2]) / (decalage*chunkSize / (float)(N - 1));
        sides[1].positions[i] = p;
        sides[1].derivatives[i] = d;


        p = shape[0].position[N-1 + N *(N - 1 - i)][2];
        d = (p - shape[0].position[N-1- decalage + N * (N - 1 - i)][2]) / (decalage*chunkSize / (float)(N - 1));
        sides[2].positions[i] = p;
        sides[2].derivatives[i] = d;


        p = shape[0].position[(N - 1 - i) + N * 0][2];
        d = -(p - shape[0].position[(N - 1 - i) + N * decalage][2]) / (decalage*chunkSize / (float)(N - 1));
        sides[3].positions[i] = p;
        sides[3].derivatives[i] = d;
    }

    for (int i = 0; i < 4; i++)
        sides[i].constrained = true;



    /*
    for (int i = 0; i < N; i++) {
        std::cout << "axe y gauche : " << sides[3].positions[i] << " / der = " << sides[3].derivatives[i] << std::endl;
    }
    */
}

void Chunk::calcul_randomterrain() {
    /*
    int nbMontagnes = -6 + rand() % 9;

    float hMax = 2;
    float hMin = 0;
    float sigmaMin = 0.4;
    float sigmaMax = 0.9;

    for (int i = 0; i < nbMontagnes; i++) {
        float h = hMin +  (hMax - hMin) * (float)rand() / RAND_MAX;
        float u = (float)rand() / RAND_MAX;
        float v = (float)rand() / RAND_MAX;
        float sigma = sigmaMin + (sigmaMax - sigmaMin) * (float)rand() / RAND_MAX;
        hMontagne.push_back(h);
        posMontagne.push_back(vcl::vec2(u, v));
        sigmaMontagne.push_back(sigma);
    }
    */
}

void Chunk::add_arbres() {
    float threshold = 0.94;

    //std::cout << "perlin = " << vcl::noise_perlin({ 0.1 * yChunk ,0.1 * xChunk }, 4, 0.7, 0.8) << std::endl;
    float noise = vcl::noise_perlin({0.05 * xChunk * chunkSize ,0.05 * yChunk * chunkSize }, 4, 0.7, 0.8);
    //std::cout << "perlin = " << noise << std::endl;
    if (noise < 1.0) {
        float r_arbres = 0.8*sqrt(1.5f - noise);
        float marge = r_arbres / 8.0f;

        std::vector<vcl::vec2> pos2D = poissonDiscSampler(chunkSize * (xChunk - 0.5f) + marge, chunkSize * (yChunk - 0.5f) + marge, chunkSize * (0.5f + xChunk) - marge, chunkSize * (0.5f + yChunk) - marge, r_arbres, 3);
        for (int i = 0; i < pos2D.size(); i++) {
            std::vector<vcl::vec3> result = get_posOnTerrain(pos2D[i]);
            if (result[1][2] >= threshold) {
                posObjets.push_back(result[0]);
                nObjets.push_back(result[1]);
                rotObjets.push_back(rotation_between_vector(vcl::vec3(0, 0, 1), normalize(vcl::vec3(0, 0, 1) + result[1])));

                float noise2 = vcl::noise_perlin({ 0.5 * posObjets[i][0], 0.5 * posObjets[i][1] }, 5, 0.8, 0.8);
                int index = 0;
                if (noise2 < 1.3) index = 1;
                indexObjets.push_back(index);
            }
        }
    }
    else if ((noise > 1.7) && (noise < 2.0)) {
        float r_arbres = 1.0f / (0.3f + 3.0f*(noise - 1.7));
        float marge = r_arbres / 8.0f;

        std::vector<vcl::vec2> pos2D = poissonDiscSampler(chunkSize * (xChunk - 0.5f) + marge, chunkSize * (yChunk - 0.5f) + marge, chunkSize * (0.5f + xChunk) - marge, chunkSize * (0.5f + yChunk) - marge, r_arbres, 3);
        for (int i = 0; i < pos2D.size(); i++) {
            std::vector<vcl::vec3> result = get_posOnTerrain(pos2D[i]);
            if (result[1][2] >= threshold) {
                posObjets.push_back(result[0]);
                nObjets.push_back(result[1]);
                rotObjets.push_back(rotation_between_vector(vcl::vec3(0, 0, 1), normalize(vcl::vec3(0, 0, 1) + result[1])));
                indexObjets.push_back(2);
            }
        }
    }
    else if ((noise > 1.4) && ((float)rand() / RAND_MAX > 0.7)) {
        vcl::vec2 pos2D = chunkSize * vcl::vec2(xChunk - 0.5f + (float)rand() / RAND_MAX, yChunk - 0.5f + (float)rand() / RAND_MAX);
        std::vector<vcl::vec3> result = get_posOnTerrain(pos2D);
        posObjets.push_back(result[0]);
        indexObjets.push_back(3);
        nObjets.push_back(result[1]);
        rotObjets.push_back(rotation_between_vector(vcl::vec3(0, 0, 1), normalize(vcl::vec3(0, 0, 1) + result[1])));
    }
}

void Chunk::afficher(scene_environment& scene) {
    for (int i = 0; i < shape.size(); i++) {
        if (displayActivated[i] && visualComputed[i]) {
            draw(visual[i], scene);
            if (wireFrame[i]) draw_wireframe(visual[i], scene);
        }
        else if (!(visualComputed[i]))
            std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
    }

    if (arbres.size() > 0) {
        //ObjModel* arbre = arbres[1];
        for (int i = 0; i < posObjets.size(); i++) {
            //arbre->set_scale(1);
            int index = indexObjets[i];
            arbres[index]->set_rotation(rotObjets[i]);
            arbres[index]->set_pos(posObjets[i]);
            arbres[index]->afficher(scene);
        }

    }
}



void Chunk::afficher_depth_map(scene_environment& scene) {
    for (int i = 0; i < shape.size(); i++) {
        if (displayActivated[i] && visualComputed[i]) {
            draw_depth_map(visual[i], scene);
            if (wireFrame[i]) draw_wireframe(visual[i], scene);
        }
        else if (!(visualComputed[i]))
            std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
    }

    if (arbres.size() > 0) {
        //ObjModel* arbre = arbres[1];
        for (int i = 0; i < posObjets.size(); i++) {
            //arbre->set_scale(1);
            int index = indexObjets[i];
            arbres[index]->set_rotation(rotObjets[i]);
            arbres[index]->set_pos(posObjets[i]);
            arbres[index]->afficher_depth_map(scene);
        }

    }
}



void Chunk::afficher_with_shadow(scene_environment& scene) {
    for (int i = 0; i < shape.size(); i++) {
        if (displayActivated[i] && visualComputed[i]) {
            draw_with_shadow(visual[i], scene);
            if (wireFrame[i]) draw_wireframe(visual[i], scene);
        }
        else if (!(visualComputed[i]))
            std::cout << "Impossible d'afficher le sub-element " << i << " car son visual n'est pas chargé." << std::endl;
    }

    if (arbres.size() > 0) {
        //ObjModel* arbre = arbres[1];
        for (int i = 0; i < posObjets.size(); i++) {
            //arbre->set_scale(1);
            int index = indexObjets[i];
            arbres[index]->set_rotation(rotObjets[i]);
            arbres[index]->set_pos(posObjets[i]);
            arbres[index]->afficher_with_shadow(scene);
        }

    }
}