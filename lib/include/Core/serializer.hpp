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

template <typename T, typename P = float>
bool Serialize(const T *t, json &ioJson, size_t size = 0)
{
    if(size == 0) size = sizeof(T)/sizeof(P);

    if(!t || size == 0) return false;

    char *p = (char*)t;
    for(size_t i = 0; i < size; ++i)
    {
        ioJson.push_back(*(P*)(p));
        p+= sizeof(P);
    }
    return true;
}

template <typename T, typename P = float>
bool Read(T *t, const json &ioJson)
{
    if(!t) return false;
     char *p = (char*)t;
    for (json::const_iterator it = ioJson.cbegin(); it != ioJson.cend(); ++it)
    {
        *p = (P)*it;
        p+= sizeof(P);
    }
    return true;
}

}

#endif // SERIALIZER_HPP

