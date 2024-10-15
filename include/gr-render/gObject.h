#pragma once

namespace grr {
    class gObject {
    public:
        virtual void bind() = 0;

        virtual void unbind() = 0;

        const int getIndex() const;
    private:
        int m_index;
    };
} // namespace grr

