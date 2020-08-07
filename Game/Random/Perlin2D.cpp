#include "Perlin2D.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>

PerlinNoise2D::PerlinNoise2D(int height, int width, int seed, int octaves, float scalingBias) {
    this->noiseSeed = new float[height * width];
    this->perlinNoise = new float[height * width];

    this->height = height;
    this->width = width;
    this->seed = seed;
    this->octaves = octaves;
    this->scalingBias = scalingBias;

    std::srand(seed);
    //std::rand();
    //std::srand(std::rand());
    for (int i = 0; i < height * width; ++i) noiseSeed[i] = std::rand() / float(RAND_MAX);

    generatePerlinNoise2D();
}
PerlinNoise2D::~PerlinNoise2D() {
    delete[] noiseSeed;
    delete[] perlinNoise;
}

void PerlinNoise2D::generatePerlinNoise2D() {
    int lim = std::max(height, width);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float noise = 0;
            float scale = 1;
            float scaleAccum = 0;

            for (int o = 0; o < octaves; o++) {
                int pitch = lim >> o;
                if (!pitch) continue;

                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;

                int sampleX2 = (sampleX1 + pitch) % width;
                int sampleY2 = (sampleY1 + pitch) % height;

                float blendX = (x - sampleX1) / float(pitch);
                float blendY = (y - sampleY1) / float(pitch);

                float sampleT = (1 - blendX) * noiseSeed[sampleY1 * width + sampleX1] + blendX * noiseSeed[sampleY1 * width + sampleX2];
                float sampleB = (1 - blendX) * noiseSeed[sampleY2 * width + sampleX1] + blendX * noiseSeed[sampleY2 * width + sampleX2];

                noise += ((1 - blendY) * sampleT + blendY * sampleB) * scale;
                scaleAccum += scale;
                scale /= scalingBias;
            }
            perlinNoise[y * width + x] = noise / scaleAccum;
            if(x==0 && y==0)std::cout <<noiseSeed[0]<<" "<< noise / scaleAccum << std::endl;
        }
    }
}

float PerlinNoise2D::getPerlin(int x,int y){
    return perlinNoise[y * width + x];
}