#pragma once

#include "buffer_element.hpp"

#include <vector>

namespace gr
{
    class buffer_layout
    {
    public:
        buffer_layout() {}
        buffer_layout(const std::initializer_list<buffer_element>& elements) 
            : m_elements(elements) {
            calculate_offsets_and_stride();
        }

        uint32_t get_stride() const { return m_stride; }
        const std::vector<buffer_element>& get_elements() const { return m_elements; }

        std::vector<buffer_element>::iterator begin() { return m_elements.begin(); }
        std::vector<buffer_element>::iterator end() { return m_elements.end(); }
        std::vector<buffer_element>::const_iterator begin() const { return m_elements.begin(); }
        std::vector<buffer_element>::const_iterator end() const { return m_elements.end(); }

    private:
        void calculate_offsets_and_stride() {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements) {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        std::vector<buffer_element> m_elements;
        uint32_t m_stride = 0;
    };
}

