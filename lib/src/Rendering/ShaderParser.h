#pragma once
#include "Rendering/Shader.h"
#include <memory>
#include <unordered_map>
namespace fm {
class ShaderParser {
    

    
public:
    ShaderParser();
    std::tuple<std::string, std::string> parse(const std::string& name);
    void includeFiles(std::string& content);
    std::string readContentFile(const std::string& name);
    void readPragmas(std::string& content);
    bool isFunction(const std::string &line, std::string &name);
    
    static unsigned int split(const std::string& txt, 
    std::vector<std::string>& strs, char ch, size_t initPos = 0);
    static std::string findStringBetweenChar(const std::string &line, char c);
    static bool replace(std::string& str, const std::string& from, const std::string& to);
    
    private:
    void argsToLayout(std::string &currentLine, bool isVertex, bool parse = true);
    std::unordered_map<std::string, std::string> pragmas;
    std::string nameFile;

    std::string contentVertex;
    std::string contentFrag;
};
}