#pragma once
#include <algorithm>
#include <array>
#include <type_traits>

template<typename T, size_t Sz>
class Med_Filter {
    static_assert(std::is_arithmetic<T>::value, "Med_Filter requires a numeric type");
    static_assert(Sz % 2 == 1, "Size must be odd");

public:
    Med_Filter() : _filled(false), _idx(0) {
        _buffer.fill(0);
    }

    T filter(T value) {
        _buffer[_idx++] = value;
        if (_idx >= Sz) {
            _idx = 0;
            _filled = true;
        }

        size_t count = _filled ? Sz : _idx;
        T tmp[Sz];
        for (size_t i = 0; i < count; ++i)
            tmp[i] = _buffer[i];

        std::nth_element(tmp, tmp + count / 2, tmp + count);
        return tmp[count / 2];
    }

    void reset() {
        _filled = false;
        _idx = 0;
    }

private:
    bool _filled;
    size_t _idx;
    std::array<T, Sz> _buffer;
};
