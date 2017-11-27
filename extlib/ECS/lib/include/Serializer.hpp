#pragma once
#include <string>
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;

#define SERIALIZE_VALUE(X) serializeTemp.write(#X, X);

class Serializer {
    public:
        Serializer() {
            
        }
        ~Serializer() {}
        template <typename T>
            void write(const std::string &name, T value) {
                j[name] = value;
            }

            void write(const std::string &name, Serializer value) {
                j[name] = value.j;
            }

        void writeToFile(const std::string &pathFile) {
            std::ofstream of(pathFile);
            of << j;
            of.close();
        }


    private:
        json j;
};
