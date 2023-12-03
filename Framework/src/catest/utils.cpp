#include <utils.h>
#include <cmath>

namespace catest
{
    bool fleq(float a, float b, float epsilon = 0.0000001f) 
    {
        return abs(a - b) <= epsilon;
    }
}