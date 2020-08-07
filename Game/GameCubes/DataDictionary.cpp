#include "DataDictionary.hpp"

#include <filesystem>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>

#include "GameCubes/Cube.hpp"
using namespace GameModel;
static std::unordered_map<std::string, Cube> cubeDictionary;
static std::unordered_map<std::string, Texture> textureDictionary;
static const std::string pathToTextureFolder = "resources/Textures";
static const std::set<std::string> imageExtensions = {".jpeg", ".jpg", ".png"};
static const std::string pathToCubeDescription = "resources/Cubes/cubes.txt";

static void loadTextures(){
    for (const auto& entry : std::filesystem::directory_iterator(pathToTextureFolder)) {
        auto& p = entry.path();
        if (imageExtensions.count(p.extension().generic_string())) {
            std::string fname = p.filename().string();
            std::string rname = fname.substr(0, fname.find("."));
            std::string fullPath = std::filesystem::absolute(p).string();
            textureDictionary[rname] = Texture(fullPath.c_str());
        }
    }
}

static void loadCubes(){
    std::ifstream cubefile(pathToCubeDescription);
    if(cubefile.is_open()){
        std::string cubename;
        while(cubefile>>cubename){
            std::string tname;
            std::array<const Texture*, 6> tex;
            for (int i = 0; i < 6; ++i) {
                cubefile >> tname;
                tex[i] = DataDictionary::getTexture(tname);
            }
            cubefile >> tname;
            const Texture* cubeImg = DataDictionary::getTexture(tname);
            bool b;
            cubefile >> b;
            cubeDictionary[cubename] = Cube(tex,cubeImg);
            cubeDictionary[cubename].setTransparent(b);
            std::cout << cubename;
        }
    }
    else {
        std::cerr << "The cube file could not be opened" << std::endl;
    }
}

const Cube* DataDictionary::getCube(std::string s) {
    if(cubeDictionary.empty()){
        loadCubes();
    }
    if(cubeDictionary.count(s)==0){
        std::cerr << " Unknown cube: " << s << std::endl;
        return nullptr;
    }
    return &cubeDictionary[s];
}
const Texture * DataDictionary::getTexture( std::string name){
    if(textureDictionary.empty()){
        loadTextures();
    }
    if(textureDictionary.count(name)==0){
        std::cerr << "Unknown texture: " << name << std::endl;
        return nullptr;
    }
    return &textureDictionary[name];
}

