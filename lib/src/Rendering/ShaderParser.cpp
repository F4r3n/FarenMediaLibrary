#include "Rendering/ShaderParser.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> // std::min

using namespace fm;

ShaderParser::ShaderParser() {
}

std::tuple<std::string, std::string> ShaderParser::parse(const std::string& name) {
    this->nameFile = name;
    // includeFiles();
    std::string content = readContentFile(name);
    includeFiles(content);
    readPragmas(content);

    return std::forward_as_tuple(contentVertex, contentFrag);
}


std::size_t min(std::size_t a, std::size_t b) {
    return a > b ? b : a;
}

unsigned int ShaderParser::split(const std::string& txt, std::vector<std::string>& strs, char ch, size_t initP) {

    size_t pos = txt.find(ch, initP);
    size_t initialPos = initP;
    strs.clear();

    while(pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
    return strs.size();
}

bool ShaderParser::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void ShaderParser::argsToLayout(std::string& currentLine, bool isVertex, bool parse) {
    std::string finalLine = "";
    std::size_t pos = 0;

    pos = currentLine.find("(");
    size_t posEnd = currentLine.find(")");
    std::string lineArgs = currentLine.substr(pos + 1, posEnd - pos - 1);

    std::vector<std::string> args;
    split(lineArgs, args, ',');
    if(parse) {
        for(unsigned int i = 0; i < args.size(); i++) {

            std::string r;
            size_t pos_ = args[i].find("_");
            r = args[i].substr(pos_ + 1, args[i].find(" ", pos_) - pos_);
            std::string n = args[i].replace(pos_, args[i].find(" ", pos_) - pos_, "");

            if(isVertex && n.find("out", (size_t)(pos_ - 3), 3) != std::string::npos) {
                finalLine += n + ";\n";
            } else if(n.find("in", (size_t)(pos_ - 2), 2) != std::string::npos) {
                finalLine += "layout(location = " + r + ") " + n + ";\n";
            }
        }
    }

    finalLine += "void main() {";
    currentLine = finalLine;
}

void ShaderParser::readPragmas(std::string& content) {
    // std::string names[2];
    std::istringstream f(content);
    std::string line;
    int numberBraces = 0;
    bool insideVertFunction = false;
    bool insideFragFunction = false;

    int numberPragmasFound = 0;
    bool vertexIsOver = false;
    bool fragIsOver = false;

    // std::cout << content << std::endl;
    while(std::getline(f, line)) {
        std::size_t pos = 0;

        if((pos = line.find("#pragma")) != std::string::npos) {
            line.erase(line.begin(), line.begin() + 8);
            std::vector<std::string> v;
            split(line, v, ':');

            if(v.size() == 2) {
                pragmas[v[0]] = v[1];
            }
            numberPragmasFound++;
            continue;
        }
        // Search functions
        if(numberPragmasFound >= 2) {

            if(numberBraces == 0 && (!insideFragFunction || !insideVertFunction)) {
                std::string vertName = pragmas["vert"];
                std::string fragName = pragmas["frag"];

                if(!vertexIsOver && (pos = line.find(vertName)) != std::string::npos) {
                    insideVertFunction = true;
                    argsToLayout(line, true, false);

                } else if(!fragIsOver && (pos = line.find(fragName)) != std::string::npos) {
                    insideFragFunction = true;
                    argsToLayout(line, false, false);
                }
            }
            if(insideVertFunction) {
                contentVertex += line + "\n";
            } else if(insideFragFunction) {
                contentFrag += line + "\n";
            }

            if(!insideVertFunction && !vertexIsOver) {
                contentVertex += line + "\n";
            }

            if(!insideFragFunction && !fragIsOver && vertexIsOver) {
                contentFrag += line + "\n";
            }

            if(line.find("{") != std::string::npos) {
                numberBraces++;
            } else if(line.find("}") != std::string::npos) {
                numberBraces--;
            }

            if(numberBraces == 0 && insideFragFunction) {
                insideFragFunction = false;
                fragIsOver = true;
            }
            if(numberBraces == 0 && insideVertFunction) {
                insideVertFunction = false;
                vertexIsOver = true;
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
        // std::cerr << "Error file not found" << std::endl;
    }
    return content;
}

std::string ShaderParser::findStringBetweenChar(const std::string& str, char c) {
    unsigned first = str.find(c);
    unsigned last = str.find_last_of(c);
    std::string strNew = str.substr(first + 1, last - first - 1);
    return strNew;
}

bool ShaderParser::isFunction(const std::string& line, std::string& name) {
    return false;
}

void ShaderParser::includeFiles(std::string& content) {

    std::istringstream f(content);
    std::string line;
    std::string finalFile = "";
    while(std::getline(f, line)) {
        std::size_t pos = 0;

        if((pos = line.find("#include")) != std::string::npos) {
            std::string nameFile = findStringBetweenChar(line, '"');
            std::ifstream ifs(nameFile);
            std::string contentInclude((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            includeFiles(contentInclude);
            finalFile += contentInclude + "\n";
        } else {
            finalFile += line + "\n";
        }
    }
    content = finalFile;
}
