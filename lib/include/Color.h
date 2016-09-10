#pragma once
#include <vector>
namespace fm
{

class Color
{
public:
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    ~Color();
    float r = 0, g = 0, b = 0;
    float a = 1;
    float mean();
    void RGB2HSV(Color& color);

private:
    template <typename T> T max(std::vector<T> vals)
    {
        T result = vals[0];
        for(int i = 1; i < vals.size(); ++i) {
            if(vals[i] > result)
                result = vals[i];
        }
        return result;
    }

    template <typename T> T min(std::vector<T> vals)
    {
        T result = vals[0];
        for(int i = 1; i < vals.size(); ++i) {
            if(vals[i] < result)
                result = vals[i];
        }
        return result;
    }
};
}
