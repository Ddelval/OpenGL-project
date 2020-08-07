#include "Perlin1D.hpp"

PerlinNoise1D::PerlinNoise1D(int size, int seed, int octaves, float scaling) {
    noiseSeed = new float[size];
    perlinNoise = new float[size];
    srand(seed);
    for (int i = 0; i < size; ++i) {
        noiseSeed[i] = std::rand() / float(RAND_MAX);
    }
    this->seed = seed;
    this->octaves = octaves;
    this->fScalingBias = scaling;
    generatePerlin1D(size, noiseSeed, this->octaves, perlinNoise);
}

float PerlinNoise1D::getPerlin(int x) {
    return this->perlinNoise[x];
}
void PerlinNoise1D::generatePerlin1D(int size, float* seeds, int octaves, float* output) {
    for (int i = 0; i < size; ++i) {
        float accum = 0.0f;
        float scale = 1.0f;
        float rangeScaling = 0;

        for (int o = 0; o < octaves; ++o) {
            int nPitch = size >> o;
            if (nPitch == 0) break;
            int nSample1 = (i / nPitch) * nPitch;
            int nSample2 = (nSample1 + nPitch) % size;

            float fBlend = float(i - nSample1) / float(nPitch);
            float fSample = (1.0f - fBlend) * seeds[nSample1] + fBlend * seeds[nSample2];
            accum += fSample * scale;
            rangeScaling += scale;
            scale /= fScalingBias;
        }
        output[i] = accum / rangeScaling;
    }
}


PerlinNoise1D::~PerlinNoise1D() {
    delete[] noiseSeed;
    delete[] perlinNoise;
}