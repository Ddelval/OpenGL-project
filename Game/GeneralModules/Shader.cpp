#include "GeneralModules/Shader.hpp"

//#include <unistd.h>
#pragma warning(disable : 4996)

using namespace std;
static unique_ptr<char[]> readFromFile(const char *file) {
    FILE *f = fopen(file, "rb");
    if (f == NULL) {
        perror("Shader loading");
        fflush(stderr);
        exit(1);
        /*char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working dir: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }*/
    }
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    printf("%d\n", len);
    fflush(stdout);
    unique_ptr<char[]> c(new char[len + 1]);

    rewind(f);
    fread(c.get(), sizeof(char), len, f);
    c.get()[len] = 0;
    return move(c);
}

void checkError(string name, uint resource, GLenum type) {
    int success;
    if (name == "linking")
        glGetProgramiv(resource, type, &success);
    else
        glGetShaderiv(resource, type, &success);
    if (!success) {
        char infoLog[512];

        if (name == "linking") {
            glGetProgramInfoLog(resource, 512, NULL, infoLog);
        } else {
            glGetShaderInfoLog(resource, 512, NULL, infoLog);
        }
        cerr << "Error in " << name << endl
             << infoLog << endl;
        exit(1);
    }
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    auto verPtr = readFromFile(vertexPath);
    const char *verC = verPtr.get();
    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &verC, NULL);
    glCompileShader(vertexShader);
    checkError("vertex shader", vertexShader, GL_COMPILE_STATUS);

    auto fragPtr = readFromFile(fragmentPath);
    const char *fragC = fragPtr.get();
    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragC, NULL);
    glCompileShader(fragmentShader);
    checkError("fragment shader", fragmentShader, GL_COMPILE_STATUS);

    this->id = glCreateProgram();
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);
    checkError("linking", this->id, GL_LINK_STATUS);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(this->id);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setmat3(const std::string &name, glm::mat3 value) const {
    glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setmat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setvec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setIntArray(const std::string &name, const std::vector<int> &val) const {
    glUniform1iv(glGetUniformLocation(this->id, name.c_str()), val.size(), val.data());
}

void Shader::setIntArray(const std::string &name, const int *pos, int len) const {
    glUniform1iv(glGetUniformLocation(this->id, name.c_str()), len * sizeof(int), pos);
}