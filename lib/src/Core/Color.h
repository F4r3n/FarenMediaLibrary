#pragma once
#include <nlohmann/json_fwd.hpp>


namespace fm {

class Color {
public:
    Color() {
        r = 1;
        g = 1;
        b = 1;
        a = 1;
    }
    Color(float r, float g, float b);
    Color(int hex);
    Color(float r, float g, float b, float a = 1);
    ~Color();
    float r = 0, g = 0, b = 0;
    float a = 1;
    float mean();


private:
    template <typename T> T max(std::vector<T> vals) {
        T result = vals[0];
        for(unsigned int i = 1; i < vals.size(); ++i) {
            if(vals[i] > result)
                result = vals[i];
        }
        return result;
    }

    template <typename T> T min(std::vector<T> vals) {
        T result = vals[0];
        for(unsigned int i = 1; i < vals.size(); ++i) {
            if(vals[i] < result)
                result = vals[i];
        }
        return result;
    }
};



}

namespace fm
{
void to_json(nlohmann::json& j, const Color& p);
void from_json(const nlohmann::json& j, Color& p);
}
