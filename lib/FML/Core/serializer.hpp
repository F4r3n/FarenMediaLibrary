#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP
#include <nlohmann/json_fwd.hpp>
namespace fm
{
class Serializer
{
    public:
        virtual bool Serialize(nlohmann::json &ioJson) const = 0;
        virtual bool Read(const nlohmann::json &inJSON) = 0;
        virtual ~Serializer() = default;
};
}

#endif // SERIALIZER_HPP

