#pragma once
#include "Rendering/Shader.h"
#include <memory>
#include <vector>
namespace fm {
class ShaderParser {
    public:
ShaderParser();
    void parse(const std::string& name, std::shared_ptr<Shader> shader);
    void parse(const std::string& name, Shader& shader);
    void includeFiles();
    std::string readContentFile(const std::string &name);
    void findFragAndVert(std::string &content);
    static unsigned int split(const std::string& txt, std::vector<std::string>& strs, char ch);
    private :
 std::string nameFile;
 
 std::string contentVertex;
 std::string contentFrag;
 
 
};
}