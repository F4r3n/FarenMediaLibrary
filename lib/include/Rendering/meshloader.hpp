#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <string>

namespace fm
{
class MeshLoader
{
    public:
        static bool Load(const std::string &inFileName);
};
}

#endif // MESHLOADER_HPP

