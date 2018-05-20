#pragma once
#include <string>
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;

#define SERIALIZE_VALUE(X) serializeTemp.write(#X, X);

class Serializer {
        virtual bool Serialize(json &ioJson) const = 0;
        virtual bool Read(const json &inJSON) = 0;
};
