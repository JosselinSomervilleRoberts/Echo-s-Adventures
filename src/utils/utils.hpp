#pragma once

#include "vcl/vcl.hpp"
#include "scene_environment.hpp"
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>


using namespace vcl;

inline std::vector<std::string> read_lines(std::string nameFile)
{
	std::ifstream file(nameFile);
	std::string str;
	std::vector<std::string> result;
	while (std::getline(file, str)) {
		result.push_back(str);
	}
	return result;
}


inline std::vector<std::string> split(std::string s, std::string delimiter)
{
	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		result.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	result.push_back(s);
	return result;
}




inline void eraseAllSubStr(std::string& mainStr, const std::string& toErase)
{
	size_t pos = std::string::npos;
	while ((pos = mainStr.find(toErase)) != std::string::npos)
		mainStr.erase(pos, toErase.length());
}

inline void eraseSubStrings(std::string& mainStr, const std::vector<std::string>& strList)
{
	std::for_each(strList.begin(), strList.end(), std::bind(eraseAllSubStr, std::ref(mainStr), std::placeholders::_1));
}


inline float get_norme(vcl::vec3 v) {
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

inline float get_norme2(vcl::vec2 v) {
	return pow(v[0], 2) + pow(v[1], 2);
}

inline float get_norme(vcl::vec2 v) {
	return sqrt(get_norme2(v));
}

inline void print(std::string s) {
	std::cout << s << std::endl;
}

inline void print(float s) {
	std::cout << s << std::endl;
}

inline void opengl_uniform(GLuint shader, scene_environment const& current_scene)
{
	opengl_uniform(shader, "projection", current_scene.projection);
	opengl_uniform(shader, "view", current_scene.camera.matrix_view());

    if (((int)(shader) == 3) || ((int)(shader) == 15)) {
        // Adapt the uniform values send to the shader
        int const N_spotlight = current_scene.spotlight_color.size();
        GLint const location_color = glGetUniformLocation(shader, "spotlight_color");
        GLint const location_position = glGetUniformLocation(shader, "spotlight_position");
        glUniform3fv(location_color, N_spotlight, ptr(current_scene.spotlight_color[0]));
        glUniform3fv(location_position, N_spotlight, ptr(current_scene.spotlight_position[0]));

        /** Note: Here we use the raw OpenGL call to glUniform3fv allowing us to pass a vector of data (here an array of 5 positions and 5 colors) */

        GLint const location_falloff = glGetUniformLocation(shader, "spotlight_falloff");
        glUniform1fv(location_falloff, N_spotlight, ptr(current_scene.spotlight_falloff[0]));
        GLint const location_intensity = glGetUniformLocation(shader, "spotlight_intensity");
        glUniform1fv(location_intensity, N_spotlight, ptr(current_scene.spotlight_intensity[0]));
        //glUniform3fv(shader, "spotlight_falloff", ptr(current_scene.spotlight_falloff[0]));
        opengl_uniform(shader, "fog_falloff", current_scene.fog_falloff);
        opengl_uniform(shader, "N_lights", current_scene.N_lights);
    }
    else {
        opengl_uniform(shader, "light", current_scene.spotlight_position[0], false);
    }
}

inline float gaussian(float x, float mu, float sigma) {
	return std::exp(-0.5 * pow((x - mu) / sigma, 2)) / (sigma * sqrt(2 * vcl::pi));
}

inline vcl::vec3 produit_vect(vcl::vec3 a, vcl::vec3 b) {
	return vcl::vec3(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

inline float produit_scal(vcl::vec3 a, vcl::vec3 b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline vcl::vec3 normalize(vcl::vec3 a) {
	return a / get_norme(a);
}


inline std::vector<vcl::vec2> poissonDiscSampler(float sx, float sy, float ex, float ey, float r, int k) {
    const float r2 = r * r;
    const float cellSize = r / sqrt(2);
    const float width = ex - sx;
    const float height = ey - sy;
    const int gridWidth = ceil(width / cellSize);
    const int gridHeight = ceil(height / cellSize);
    const int gridSize = gridWidth * gridHeight;
    int* grid = new int[gridSize];
    std::fill_n(grid, gridWidth * gridHeight, -1);

    std::vector<vcl::vec2> queue;
    std::vector<vcl::vec2> points;
    queue.push_back(vcl::vec2((sx + ex) / 2.0f, (sy + ey) / 2.0f));

    int i;
    vcl::vec2 parent;
    float seed;
    const float epsilon = 0.00001f;
    float angle;
    vcl::vec2 candidate;
    int ix;
    int iy;
    int index;
    bool ok;
    int j;
    bool trouve;

    // (float) rand()/RAND_MAX
    while (queue.size() > 0) {
        i = rand() % queue.size();
        parent = queue[i];
        seed = (float)rand() / RAND_MAX;

        // On cherche un nouveau candidat
        j = 0;
        trouve = false;
        while (!trouve && j < k) {
            // On choisit un point
            angle = 2 * vcl::pi * (seed + 1.0 * j / k);
            candidate = parent + (r + epsilon) * vcl::vec2(std::cos(angle), std::sin(angle));

            // On vérifie la distance
            ix = (gridWidth - 1) * (candidate[0] - sx) / width;
            iy = (gridHeight - 1) * (candidate[1] - sy) / height;
            if ((candidate[0] > sx) && (candidate[0] < ex) && (candidate[1] > sy) && (candidate[1] < ey)) {
                ok = true;
                for (int xx = std::max(0, ix - 1); xx <= std::min(gridWidth - 1, ix + 1); xx++) {
                    for (int yy = std::max(0, iy - 1); yy <= std::min(gridHeight - 1, yy + 1); yy++) {
                        index = xx + yy * gridWidth;
                        if (grid[index] != -1) { // Il y a quelqu(un dans la case
                            if (get_norme2(points[grid[index]] - candidate) < r2) {
                                ok = false;
                            }
                        }
                    }
                }
            }
            else {
                ok = false;
            }
            j++;

            // Si on a trouvé un candidat valable
            if (ok) {
                grid[ix + iy * gridWidth] = points.size();
                points.push_back(candidate);
                queue.push_back(candidate);
                trouve = true;
            }
        }
        if (!trouve) { // Si on a trouvé aucun candidat alors on enlève de la queue le point
            queue.erase(queue.begin() + i);
        }
    }

    return points;
}