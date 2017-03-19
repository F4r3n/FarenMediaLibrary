
#include "Rendering/Shader.h"

using namespace fm;

Shader::Shader() {
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    this->vertex = vertexCode;
    this->fragment = fragmentCode;
}

Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode) {
    this->vertex = vertexCode;
    this->fragment = fragmentCode;
}

bool Shader::compile() {
    const GLchar* vShaderCode = vertex.c_str();
    const GLchar* fShaderCode = fragment.c_str();
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << vShaderCode << std::endl;
        return false;
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << fShaderCode << std::endl;

        return false;
    }
    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return true;
}

Shader* Shader::Use() {
    glUseProgram(this->Program);
    return this;
}

Shader* Shader::setMatrix(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    return this;
}

Shader* Shader::setVector2f(const std::string& name, glm::vec2 vector) {
    glUniform2f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y);
    return this;
}

Shader* Shader::setVector3f(const std::string& name, glm::vec3 vector) {
    glUniform3f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y, vector.z);
    return this;
}

Shader* Shader::setFloat(const std::string& name, float val) {
    glUniform1f(glGetUniformLocation(Program, name.c_str()), val);
    return this;
}

Shader* Shader::setVector4f(const std::string& name, glm::vec4 vector) {
    glUniform4f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y, vector.z, vector.w);
    return this;
}

Shader* Shader::setInt(const std::string& name, int val) {
    glUniform1i(glGetUniformLocation(Program, name.c_str()), val);
    return this;
}

Shader* Shader::setVector2f(const std::string& name, math::Vector2f vector) {
    return setVector2f(name, glm::vec2(vector.x, vector.y));
}

Shader* Shader::setColor(const std::string& name, Color vector) {
    return setVector4f(name, glm::vec4(vector.r, vector.g, vector.b, vector.a));
}

Shader::~Shader() {
}
