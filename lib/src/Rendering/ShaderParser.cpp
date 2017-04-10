#include "Rendering/ShaderParser.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>    // std::min

using namespace fm;

ShaderParser::ShaderParser() {
}

void ShaderParser::parse(const std::string& name, std::shared_ptr<Shader> shader) {
    this->nameFile = name;
    // includeFiles();
    std::string content = readContentFile(name);
    findFragAndVert(content);
}
void ShaderParser::parse(const std::string& name, Shader& shader) {
    this->nameFile = name;
    includeFiles();
}

std::size_t min(std::size_t a, std::size_t b) {
    return a > b ? b :a;
}

unsigned int ShaderParser::split(const std::string& txt, std::vector<std::string>& strs, char ch) {
    unsigned int pos = txt.find(ch);
    unsigned int initialPos = 0;
    //std::cout << txt << " " << ch << " " << pos << std::endl;
    strs.clear();

    // Decompose statement
    if(pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
//std::cout << strs[0] << " " << strs[1] << std::endl;
    return strs.size();
}

void ShaderParser::findFragAndVert(std::string& content) {
    std::string names[2];
    std::istringstream f(content);
    std::string line;
    std::cout << content << std::endl;
    while(std::getline(f, line)) {
        std::size_t pos = 0;
        if((pos = line.find("#pragma")) != std::string::npos) {
            line.erase(line.begin(), line.begin() + 8);
            std::cout << line << " " << pos << std::endl;
            std::vector<std::string> v;
            split(line, v, ':');
            if(v.size() == 2) {
                if(v[0].find("vert") != std::string::npos) {
                    names[0] = v[1];
                } else if(v[0].find("frag") != std::string::npos) {
                    names[1] = v[1];
                }
            }
        }
        // std::cout << line << std::endl;
    }
}

std::string ShaderParser::readContentFile(const std::string& name) {

    std::ifstream file;
    std::string content = "";
    // ensures ifstream objects can throw exceptions:
    file.exceptions(std::ifstream::badbit);
    try {
        // Open files
        file.open(name);
        std::stringstream fileStream;
        // Read file's buffer contents into streams
        fileStream << file.rdbuf();
        // close file handlers
        file.close();
        // Convert stream into string
        content = fileStream.str();
    } catch(std::ifstream::failure e) {
        std::cerr << "Error file not found" << std::endl;
    }
    return content;
}

void ShaderParser::includeFiles() {
}