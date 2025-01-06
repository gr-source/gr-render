#pragma once

#include <cstdint>

namespace grr {

    class Shader {
    public:
        Shader();
        ~Shader();

    private:
        std::uint32_t mID;
    };
    
} // namespace grr
