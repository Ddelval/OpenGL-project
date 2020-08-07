#ifndef PERLIN_2D_HPP
#define PERLIN_2D_HPP
class PerlinNoise2D {
   public:
    PerlinNoise2D(int height, int width, int seed, int octaves, float scalingBias);
    ~PerlinNoise2D();

    /**
     * @brief Get the perlin noise value for the given coordinates
     * 
     * @param x Coordinates in the "width" axis
     * @param y Coordinates in the "height" axis
     * @return Value of the perlin noise
     */
    float getPerlin(int x, int y);

   private:
    int height;
    int width;
    int seed;
    int octaves;
    float scalingBias;

    float* noiseSeed;
    float* perlinNoise;

    void generatePerlinNoise2D();
};

#endif