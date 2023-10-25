#include "Core/Color.h"
#include <nlohmann/json.hpp>
using namespace fm;
Color::Color(float r, float g, float b)
    : r(r)
    , g(g)
    , b(b)
{
}
Color::Color(float r, float g, float b, float a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

Color::Color(int hex) {
    r = ((hex >> 16) & 0xFF)/255.0f;
    g = ((hex >> 8) & 0xFF)/255.0f;
    b = (hex & 0xFF)/255.0f;
}

float Color::mean()
{
    return (r + g + b) / 3.0f;
}

Color::~Color()
{
}


namespace fm
{
using nlohmann::json;


void to_json(json& j, const Color& p)
{
        j = json{{"r", p.r}, {"g", p.g}, {"b", p.b}, {"a", p.a}};
}

 void from_json(const json& j, Color& p)
 {
        p.r = j.at("r").get<float>();
        p.g = j.at("g").get<float>();
        p.b = j.at("b").get<float>();
        p.a = j.at("a").get<float>();

}
}



