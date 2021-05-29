
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
        vect.push_back(objects["rune_rouge"]);
        vect.push_back(objects["rune_vert"]);
        vect.push_back(objects["rune_bleu"]);
        vect.push_back(objects["house1"]);
        vect.push_back(objects["house2"]);
        vect.push_back(objects["house3"]);
        vect.push_back(objects["house4"]);
        vect.push_back(objects["house5"]);
        vect.push_back(objects["house6"]);
        vect.push_back(objects["house7"]);
        vect.push_back(objects["lamp"]);
        vect.push_back(objects["campfire"]);
        vect.push_back(objects["mushroom1"]);
        vect.push_back(objects["mushroom2"]);
        vect.push_back(objects["cactus1"]);
        vect.push_back(objects["cactus2"]);
        vect.push_back(objects["cactus3"]);
        vect.push_back(objects["pyramid"]);
        vect.push_back(objects["tower"]);
        vect.push_back(objects["rubber_duck"]);
        Chunk c = Chunk(x, y, N, chunkSize, textureLoader, vect);
        //std::cout << "new chunk N=" << N << std::endl;

        // Voisin de droite
        // set_side(1, get(3))
        /*
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
        */
        c.finish_creation();
        chunks[posNewChunk] = c;
    }
}

void Terrain::calcul_shape() {

}

void Terrain::afficher(scene_environment& scene) {
    afficher(scene, 0, distMaxRender);
}

void Terrain::afficher(scene_environment& scene, float distMin, float distMax) {
    //vcl::vec3 posCam = scene.camera.position_camera;
    vcl::vec3 posCam = scene.camera.center_of_rotation;
    int xCam = posCam[0] / chunkSize;
    int yCam = posCam[1] / chunkSize;
    int renderDistance = distMax / chunkSize;
    int minRenderDistance = distMin / chunkSize;


    if (toBeLoaded.size() > 0) {
        newChunk(toBeLoaded[0].x, toBeLoaded[0].y);
        toBeLoaded.erase(toBeLoaded.begin());

        //scene.ajouterLumiere(chunks[toBeLoaded[0]].posLumiere[i], chunks[toBeLoaded[0]].couleurLumiere[i], chunks[toBeLoaded[0]].falloffLumiere[i], chunks[toBeLoaded[0]].intensiteLumiere[i]);
    }
    
    float ratioDestruct = 2;
    std::map<chunkPos, Chunk>::iterator it = chunks.begin();
    std::vector<chunkPos> toBeErased;

    while (it != chunks.end())
    {
        int x = it->first.x;
        int y = it->first.y;
        float d = sqrt((x - xCam) * (x - xCam) + (y - yCam) * (y - yCam));
        if (d >= ratioDestruct * renderDistance)  
            toBeErased.push_back(it->first);
         it++;
    }

    
    for (int i = 0; i < toBeErased.size(); i++) {
        chunks.erase(toBeErased[i]);
        //delete chunks[toBeErased[i]];
    }
    

    
    scene.clearLumiere();
    if (eyesActivated) {
        scene.ajouterLumiere(posLoup, vcl::vec3(1, 0.5, 0.5), 0.05f, 18);
    }
    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            float d2 = x*x + y * y;
            if ((d2 < renderDistance * renderDistance) && (d2 >= minRenderDistance* minRenderDistance)) {
                chunkPos posChunk(x + xCam, y + yCam);
                //std::cout << chunks.size();
                if (chunks.find(posChunk) == chunks.end()) {
                    if (std::find(toBeLoaded.begin(), toBeLoaded.end(), posChunk) == toBeLoaded.end())
                        toBeLoaded.push_back(posChunk);
                }
                else {
                    //chunks[posChunk].afficher(scene);
                    
                    for (int i = 0; i < chunks[posChunk].get_N_lights(); i++)
                        scene.ajouterLumiere(chunks[posChunk].posLumiere[i], chunks[posChunk].couleurLumiere[i], chunks[posChunk].falloffLumiere[i], chunks[posChunk].intensiteLumiere[i]);
                }
            }
            
        }
    }

    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            float d2 = x * x + y * y;
            if ((d2 < renderDistance * renderDistance) && (d2 >= minRenderDistance * minRenderDistance)) {
                chunkPos posChunk(x + xCam, y + yCam);
                //std::cout << chunks.size();
                if (chunks.find(posChunk) == chunks.end()) {
                }
                else {
                    chunks[posChunk].afficher(scene);
                }
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


void Terrain::update(float dt) {
    std::map<chunkPos, Chunk>::iterator it = chunks.begin();

    while (it != chunks.end()) {
        chunks[it->first].update(dt);
        it++;
    }
}
void Terrain::load_model(std::string nameModel) {
    std::cout << "Loading " + nameModel + "...";
    ObjModel* obj = new ObjModel(nameModel, textureLoader);
    objects[nameModel] = obj;
    std::cout << " done !" << std::endl;
}

void Terrain::load_models() {
    load_model("pine1");
    load_model("pine2");
    load_model("tree1");
    load_model("rock1");
    load_model("rubber_duck");
    load_model("rune_rouge");
    load_model("rune_vert");
    load_model("rune_bleu");
    load_model("house1");
    load_model("house2");
    load_model("house3");
    load_model("house4");
    load_model("house5");
    load_model("house6");
    load_model("house7");
    load_model("lamp");
    load_model("campfire");
    load_model("mushroom1");
    load_model("mushroom2");
    load_model("cactus1");
    load_model("cactus2");
    load_model("cactus3");
    load_model("pyramid");
    load_model("tower");
}


float Terrain::get_heightOfTerrain(float x, float y) {
    int xChunk = floor(0.5f + (x / chunkSize));
    int yChunk = floor(0.5f + (y / chunkSize));

    chunkPos posChunk(xChunk, yChunk);
    if (chunks.find(posChunk) == chunks.end()) {
        //std::cout << "errrrrrrrrrrrrrrreur" << std::endl;
        return -1;
    }
    else {
        return chunks[posChunk].get_posOnTerrain(x, y)[0][2];
    }
}


std::vector<vcl::vec3> Terrain::get_posOnTerrain(float x, float y) {
    int xChunk = floor(0.5f + (x / chunkSize));
    int yChunk = floor(0.5f + (y / chunkSize));

    chunkPos posChunk(xChunk, yChunk);
    if (chunks.find(posChunk) == chunks.end()) {
        //std::cout << "errrrrrrrrrrrrrrreur" << std::endl;
        return {vcl::vec3(x,y,0), vcl::vec3(0,0,1)};
    }
    else {
        return chunks[posChunk].get_posOnTerrain(x, y);
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
    //vcl::vec3 posCam = scene.camera.position_camera;
    vcl::vec3 posCam = scene.camera.center_of_rotation;
    int xCam = posCam[0] / chunkSize;
    int yCam = posCam[1] / chunkSize;
    float renderDistance = distMaxRender / chunkSize;
    float ratioDestruct = 2;

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
    //vcl::vec3 posCam = scene.camera.position_camera;
    vcl::vec3 posCam = scene.camera.center_of_rotation;
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


vcl::vec3 Terrain::get_nearestPierre(vcl::vec3 pos_) {
    int xChunk = floor(0.5f + (pos_[0] / chunkSize));
    int yChunk = floor(0.5f + (pos_[1] / chunkSize));
    chunkPos posChunk(xChunk, yChunk);
    if (chunks.find(posChunk) == chunks.end()) {
        std::cout << "errrrrrrrrrrrrrrreur" << std::endl;
        return vcl::vec3(0, 0, 0);// -1;
    }
    else {
        return chunks[posChunk].get_nearestPierre(pos_);
    }
}

bool Terrain::has_pierre(vcl::vec3 pos_) {
    int xChunk = floor(0.5f + (pos_[0] / chunkSize));
    int yChunk = floor(0.5f + (pos_[1] / chunkSize));
    chunkPos posChunk(xChunk, yChunk);
    if (chunks.find(posChunk) == chunks.end()) {
        //std::cout << "errrrrrrrrrrrrrrreur" << std::endl;
        return false;
    }
    else {
        return chunks[posChunk].has_pierre();
    }
}