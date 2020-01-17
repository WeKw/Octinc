/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_BITBLOCK__
#define OCTINC_BITBLOCK__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace Octinc {
class bitblock {
   private:
    uint8_t* _block;
    size_t _length;
    int bit_idx = 0;

   public:
    bitblock(uint8_t* block = nullptr, size_t length = 0) {
        _block = block;
        _length = length;
    }

    void build(uint8_t* block, size_t length) {
        _block = block;
        _length = length;
    }

    void clear() {
        _block = nullptr;
        _length = 0;
    }

    void push_bits(int v, int nbit)
    /*
    Push a bit into the array.
    */
    {
        assert(nbit + bit_idx <= _length * 8);

        for (int i = 0; i < nbit; ++i) {
            int b = 1 & (v >> (nbit - i - 1));
            _block[bit_idx / 8] |= b << (7 - bit_idx % 8);
            ++bit_idx;
        }
    }

    void push_bits(long long v, int nbit) {
        assert(nbit + bit_idx <= _length * 8);

        for (int i = 0; i < nbit; ++i) {
            int b = 1 & (v >> (nbit - i - 1));
            _block[bit_idx / 8] |= b << (7 - bit_idx % 8);
            ++bit_idx;
        }
    }

    void pull_bits(int& v, int nbit)
    /*
    Pull a bit from the array.
    */
    {
        assert(nbit + bit_idx <= _length * 8);

        for (int i = 0; i < nbit; ++i) {
            int b = 1 & _block[bit_idx / 8] >> (7 - bit_idx % 8);
            v |= b << (nbit - i - 1);
            ++bit_idx;
        }
    }

    void pull_bits(long long& v, int nbit) {
        assert(nbit + bit_idx <= _length * 8);

        for (int i = 0; i < nbit; ++i) {
            int b = 1 & _block[bit_idx / 8] >> (7 - bit_idx % 8);
            v |= b << (nbit - i - 1);
            ++bit_idx;
        }
    }
};
}  // namespace Octinc

#endif
