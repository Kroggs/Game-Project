#pragma once

namespace utils
{
    float Lerp(float x, float y, float z) {
        return ((1.0f - z) * x) + (z * y);
    }
}
