
#include "terrain.hpp"

using namespace vcl;


vec3 get_pos(vcl::mesh& terrain, float u, float v) {
    // Number of samples of the terrain is N x N
    const unsigned int N = 200;

    float const x = 20 * (u - 0.5f);
    float const y = 20 * (v - 0.5f);
    int ku = (N - 1.0f) * u;
    int kv = (N - 1.0f) * v;

    float z = terrain.position[kv + N * ku][2];

    return { x,y,z };
}
/*
std::vector<vcl::vec3> generate_positions_on_terrain(int N) {
    return generate_positions_on_terrain(N, 0.9);
}

std::vector<vcl::vec3> generate_positions_on_terrain(int N, double rayon) {
    std::vector<vcl::vec3> positions;

    int nb_pos = 0;
    while (nb_pos < N) {
        vcl::vec3 pos = evaluate_terrain(rand_interval(), rand_interval());

        // Verificationd e l espace
        bool correct = true;
        int i = 0;
        while ((correct) && (i < nb_pos)) {
            if (distance_plane(pos, positions[i]) < rayon)
                correct = false;
            i++;
        }

        if (correct) {
            pos[2] -= 0.05f;
            positions.push_back(pos);
            nb_pos += 1;
        }
    }

    return positions;
}

std::vector<vcl::vec3> generate_positions_on_terrain(vcl::mesh& terrain, int N, double marge, std::vector<int>& types, std::vector<double>& rayons) {
    std::vector<vcl::vec3> positions;
    srand(time(NULL));

    int nb_pos = 0;
    while (nb_pos < N) {
        vcl::vec3 pos = get_pos(terrain, rand_interval(), rand_interval());
        int type = std::rand() % rayons.size();
        double rayon = rayons[type];

        // Verification de l espace
        bool correct = true;
        int i = 0;
        while ((correct) && (i < nb_pos)) {
            if (distance_plane(pos, positions[i]) < marge + rayon + rayons[types[i]])
                correct = false;
            i++;
        }

        if (correct) {
            //pos[2] -= 0.05f;
            positions.push_back(pos);
            nb_pos += 1;
            types.push_back(type);
        }
    }

    return positions;
}
*/
double distance_plane(vcl::vec3 pos1, vcl::vec3 pos2) {
    return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));
}




Terrain::Terrain() : Affichable() {
    chunkSize = 10;
    N = 100;
    load_models();
}


Terrain::Terrain(float chunkSize_, size_t N_) : Affichable() {
    chunkSize = chunkSize_;
    N = N_;
    load_models();
    initiateChunks();
}

Terrain::Terrain(float chunkSize_, size_t N_, TextureLoader* textureLoader_) : Affichable(textureLoader_) {
    chunkSize = chunkSize_;
    N = N_;
    load_models();
    initiateChunks();
}

void Terrain::newChunk(int x, int y) {
    
    chunkPos posNewChunk(x, y);
    
    if (chunks.find(posNewChunk) == chunks.end()) { // Le chunk n'existe pas déja
        //std::cout << "objects size = " << objects.size() << std::endl;
        std::vector<ObjModel*> vect;
        vect.push_back(objects["pine1"]);
        vect.push_back(objects["pine2"]);
        vect.push_back(objects["tree1"]);
        vect.push_back(objects["rock1"]);
        Chunk c = Chunk(x, y, N, chunkSize, textureLoader, vect);
        //std::cout << "new chunk N=" << N << std::endl;

        // Voisin de droite
        // set_side(1, get(3))
        if (chunks.find(chunkPos(x + 1, y)) != chunks.end())
            c.set_side(1, chunks[chunkPos(x + 1, y)].get_side(3).reversed());
        
        // Voisin de gauche
        // set_side(3, get(1))
        if (chunks.find(chunkPos(x - 1, y)) != chunks.end())
            c.set_side(3, chunks[chunkPos(x - 1, y)].get_side(1).reversed());

        // Voisin du bas
        // set_side(0, get(2))
        if (chunks.find(chunkPos(x, y - 1)) != chunks.end())
            c.set_side(0, chunks[chunkPos(x, y - 1)].get_side(2).reversed());

        // Voisin du haut
        // set_side(2, get(0))
        if (chunks.find(chunkPos(x, y + 1)) != chunks.end())
            c.set_side(2, chunks[chunkPos(x, y + 1)].get_side(0).reversed());

        c.finish_creation();
        chunks[posNewChunk] = c;
    }
}

void Terrain::calcul_shape() {

}

void Terrain::afficher(scene_environment& scene) {
    //std::map<chunkPos, Chunk>::iterator it = chunks.begin();
    vcl::vec3 posCam = scene.camera.position_camera;
    int xCam = posCam[0] / chunkSize;
    int yCam = posCam[1] / chunkSize;
    float renderDistance = distMaxRender / chunkSize;

    /*
    float ratioDestruct = 150;

    while (it != chunks.end())
    {
        int x = it->first.x;
        int y = it->first.y;
        float d = sqrt((x - xCam) * (x - xCam) + (y - yCam) * (y - yCam));
        if (d >= ratioDestruct * renderDistance) {
            chunks.erase(it->first);
        }
            it++;
    }*/

    if (toBeLoaded.size() > 0) {
        newChunk(toBeLoaded[0].x, toBeLoaded[0].y);
        toBeLoaded.erase(toBeLoaded.begin());
    }

    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            if (x * x + y * y < renderDistance * renderDistance) {
                chunkPos posChunk(x + xCam, y + yCam);
                if (chunks.find(posChunk) == chunks.end()) {
                    if (std::find(toBeLoaded.begin(), toBeLoaded.end(), posChunk) == toBeLoaded.end())
                        toBeLoaded.push_back(posChunk);
                }
                else
                    chunks[posChunk].afficher(scene);
            }
        }
    }
    
    /*
    int nb_chunks = 15;
    for (int y = -int(nb_chunks / 2.0); y <= int(nb_chunks / 2.0); y++) {
        for (int x = -int(nb_chunks / 2.0); x <= int(nb_chunks / 2.0); x++) {
            chunks[chunkPos(x, y)].afficher(scene);
        }
    }*/
    //chunks[chunkPos(0, -1)].afficher(scene);
    //chunks[chunkPos(0, 1)].afficher(scene);
    /*for (const auto& p : chunks) {
        p.second.afficher(scene);
    }*/
}
//for (auto const& x : symbolTable)


void Terrain::load_model(std::string nameModel) {
    ObjModel* obj = new ObjModel(nameModel, textureLoader);
    objects[nameModel] = obj;
}

void Terrain::load_models() {
    load_model("pine1");
    load_model("pine2");
    load_model("tree1");
    load_model("rock1");
    load_model("grass1");
}


float Terrain::get_heightOfTerrain(float x, float y) {
    int xChunk = floor(0.5f + (x / chunkSize));
    int yChunk = floor(0.5f + (y / chunkSize));

    chunkPos posChunk(xChunk, yChunk);
    if (chunks.find(posChunk) == chunks.end()) {
        std::cout << "errrrrrrrrrrrrrrreur" << std::endl;
        return -1;
    }
    else {
        return chunks[posChunk].get_posOnTerrain(x, y)[0][2];
    }
}

void Terrain::initiateChunks() {
    float renderDistance = distMaxRender / chunkSize;
    float rendInitial = 7;
    
    for (int x = -rendInitial; x <= rendInitial; x++) {
        for (int y = -rendInitial; y <= rendInitial; y++) {
            if (x * x + y * y < rendInitial * rendInitial) {
                newChunk(x, y);
            }
        }
    }
 
    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            if (x * x + y * y < renderDistance * renderDistance) {
                //newChunk(x, y);
                chunkPos posChunk(x, y);
                if (chunks.find(posChunk) == chunks.end()) {
                    if (std::find(toBeLoaded.begin(), toBeLoaded.end(), posChunk) == toBeLoaded.end())
                        toBeLoaded.push_back(posChunk);
                }
            }
        }
    }
}



void Terrain::afficher_depth_map(scene_environment& scene) {
    std::map<chunkPos, Chunk>::iterator it = chunks.begin();
    vcl::vec3 posCam = scene.camera.position_camera;
    int xCam = posCam[0] / chunkSize;
    int yCam = posCam[1] / chunkSize;
    float renderDistance = distMaxRender / chunkSize;
    float ratioDestruct = 150;

    while (it != chunks.end())
    {
        int x = it->first.x;
        int y = it->first.y;
        float d = sqrt((x - xCam) * (x - xCam) + (y - yCam) * (y - yCam));
        if (d >= ratioDestruct * renderDistance) {
            chunks.erase(it->first);
        }
        it++;
    }

    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            if (x * x + y * y < renderDistance * renderDistance) {
                newChunk(x + xCam, y + yCam);
                chunks[chunkPos(x + xCam, y + yCam)].afficher_depth_map(scene);
            }
        }
    }
}



void Terrain::afficher_with_shadow(scene_environment& scene) {
    std::map<chunkPos, Chunk>::iterator it = chunks.begin();
    vcl::vec3 posCam = scene.camera.position_camera;
    int xCam = posCam[0] / chunkSize;
    int yCam = posCam[1] / chunkSize;
    float renderDistance = distMaxRender / chunkSize;

    /*
    float ratioDestruct = 150;

    while (it != chunks.end())
    {
        int x = it->first.x;
        int y = it->first.y;
        float d = sqrt((x - xCam) * (x - xCam) + (y - yCam) * (y - yCam));
        if (d >= ratioDestruct * renderDistance) {
            chunks.erase(it->first);
        }
        it++;
    }*/

    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            if (x * x + y * y < renderDistance * renderDistance) {
                newChunk(x + xCam, y + yCam);
                chunks[chunkPos(x + xCam, y + yCam)].afficher_with_shadow(scene);
            }
        }
    }
}