#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <cmath>
#include <cstdlib>

class PerlinNoise1D {
   public:
    PerlinNoise1D(int size, int seed, int octaves, float scaling);
    ~PerlinNoise1D();

    float getPerlin(int x);


   private:
    int seed;
    int octaves;
    float fScalingBias;
    float* noiseSeed;
    float* perlinNoise;

    void generatePerlin1D(int size, float* seeds, int octaves, float* output);
};
#endif