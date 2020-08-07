#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "GeneralModules/utility.hpp"

class Shader {
   public:
    uint id;
    bool tex = true;
    Shader(){};
    Shader(const char *vertexPath, const char *fragmentPath);

    void use()const ;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setmat4(const std::string &name, glm::mat4 value) const;
    void setmat3(const std::string &name, glm::mat3 value) const;
    void setvec3(const std::string &name, glm::vec3 value) const;
    void setIntArray(const std::string &name, const std::vector<int>& val) const;
    void setIntArray(const std::string &name, const int *pos, int len) const;
};

#endif