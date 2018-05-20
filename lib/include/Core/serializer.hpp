#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP
#include <json.hpp>
using json = nlohmann::json;

namespace fm
{
class Serializer
{
    public:
        virtual bool Serialize(json &ioJson) const = 0;
        virtual bool Read(const json &inJSON) = 0;
        virtual ~Serializer() = default;
};

}

#endif // SERIALIZER_HPP

