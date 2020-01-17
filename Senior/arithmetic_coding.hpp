/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_ARITHMETIC_CODING_HPP
#  define OCTINC_ARITHMETIC_CODING_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

/*
Refer to Chapter 4 of Introduction to Data Compression: Arithmetic Coding
Notice:Adaptive mode arithmetic coding has some bugs.
*/
namespace Octinc {
class arithmetic_coding {
   public:
    typedef uint32_t value_type;
    typedef uint32_t count_t;
    static const size_t WORD_BITS = sizeof(value_type) * 8;
    static const size_t MSB_INDEX = WORD_BITS - 1;
    static const size_t SSB_INDEX = MSB_INDEX - 1;
    static const size_t TABLE_SIZE = 256;
    static const size_t MAX_TABLE_IDX = TABLE_SIZE - 1;
    static const value_type MSB_XOR = 1 << MSB_INDEX;

   protected:
    std::array<count_t, TABLE_SIZE> table;
    size_t _encode_bit_len;
    size_t _total_src_byte;

   public:
    arithmetic_coding() throw() {}

    enum EncodeMode {
        eStatisticMode,
        eAdaptiveMode,
    };

    size_t bit_len() { return _encode_bit_len; }

    std::vector<value_type> Encode(EncodeMode mode, std::vector<uint8_t> src) {
        switch (mode) {
            case arithmetic_coding::eStatisticMode: {
                return StatisticEncode(src);
                break;
            }
            case arithmetic_coding::eAdaptiveMode: {
                return AdaptiveEncode(src);
                break;
            }
            default: {
                return AdaptiveEncode(src);
                break;
            }
        }

        return std::vector<value_type>();
    }

    std::vector<uint8_t> Decode(EncodeMode mode, std::vector<value_type> src,
                                size_t dst_size) {
        switch (mode) {
            case arithmetic_coding::eStatisticMode: {
                return StatisticDecode(src, dst_size);
                break;
            }
            case arithmetic_coding::eAdaptiveMode: {
                return AdaptiveDecode(src, dst_size);
                break;
            }
            default: {
                return AdaptiveDecode(src, dst_size);
                break;
            }
        }

        return std::vector<uint8_t>();
    }

    template <typename __FwdIt, typename __Tp>
    __FwdIt find(__FwdIt first, __FwdIt last, const __Tp& value) {
        for (; first != last; first++)
            if (value < *first) return first;

        return last;
    }

   private:
    std::vector<value_type> AdaptiveEncode(std::vector<uint8_t> src) {
        std::vector<value_type> dst;

        for (size_t i = 0; i < TABLE_SIZE; ++i) table[i] = i + 1;

        _total_src_byte = table[MAX_TABLE_IDX];

        value_type lower = 0, upper = -1;
        size_t Scale3 = 0;
        value_type result = 0;
        size_t bits = 0;

        for (auto i : src) {
            uint64_t range = (uint64_t)upper - lower + 1;
            uint64_t lower1 =
                lower + (range * (i == 0 ? 0 : table[i - 1])) / _total_src_byte;
            uint64_t upper1 = lower + (range * table[i]) / _total_src_byte - 1;
            lower = static_cast<value_type>(lower1);
            upper = static_cast<value_type>(upper1);
            size_t LMSB = lower >> MSB_INDEX;
            size_t UMSB = upper >> MSB_INDEX;

            while (LMSB == UMSB || (lower >> SSB_INDEX & 1) &&
                                       ((upper >> SSB_INDEX & 1) == 0)) {
                if (LMSB == UMSB) {
                    result = result << 1 | LMSB;

                    if (++bits == WORD_BITS) {
                        dst.push_back(result);
                        bits = 0;
                        result = 0;
                    }

                    lower <<= 1;
                    upper = upper << 1 | 1;

                    while (Scale3 > 0) {
                        --Scale3;
                        result = result << 1 | LMSB ^ 1;

                        if (++bits == WORD_BITS) {
                            dst.push_back(result);
                            bits = 0;
                            result = 0;
                        }
                    }
                } else {
                    lower <<= 1;
                    upper = upper << 1 | 1;
                    lower ^= MSB_XOR;
                    upper ^= MSB_XOR;
                    ++Scale3;
                }

                LMSB = lower >> MSB_INDEX;
                UMSB = upper >> MSB_INDEX;
            }

            uint32_t index = i;

            while (index < TABLE_SIZE) ++table[index++];

            ++_total_src_byte;
        }

        size_t count = WORD_BITS;

        if (Scale3 > 0) {
            --count;
            size_t LMSB = lower >> MSB_INDEX;
            result <<= 1;
            result |= LMSB;

            if (++bits == WORD_BITS) {
                dst.push_back(result);
                bits = 0;
                result = 0;
            }

            LMSB ^= 1;

            while (Scale3--) {
                result <<= 1;
                result |= LMSB;

                if (++bits == WORD_BITS) {
                    dst.push_back(result);
                    bits = 0;
                    result = 0;
                }
            }
        }

        for (int i = count - 1; i >= 0; --i) {
            size_t LMSB = lower >> i & 1;
            result <<= 1;
            result |= LMSB;

            if (++bits == WORD_BITS) {
                dst.push_back(result);
                bits = 0;
                result = 0;
            }
        }

        size_t bitlen = bits + WORD_BITS * dst.size();

        if (bits > 0) {
            result <<= (WORD_BITS - bits);
            dst.push_back(result);
        }

        _encode_bit_len = bitlen;
        return dst;
    }

    std::vector<value_type> StatisticEncode(std::vector<uint8_t> src) {
        std::vector<value_type> dst;
        table.fill(0);

        for (auto& c : src) ++table[c];

        for (size_t i = 1; i < TABLE_SIZE; ++i) table[i] += table[i - 1];

        _total_src_byte = table[MAX_TABLE_IDX];

        value_type lower = 0, upper = -1;
        size_t Scale3 = 0;
        value_type result = 0;
        size_t bits = 0;

        for (auto i : src) {
            uint64_t range = (uint64_t)upper - lower + 1;
            uint64_t lower1 =
                lower + (range * (i == 0 ? 0 : table[i - 1])) / _total_src_byte;
            uint64_t upper1 = lower + range * table[i] / _total_src_byte - 1;
            lower = static_cast<value_type>(lower1);
            upper = static_cast<value_type>(upper1);
            size_t LMSB = lower >> MSB_INDEX;
            size_t UMSB = upper >> MSB_INDEX;

            while (LMSB == UMSB || (lower >> SSB_INDEX & 1) &&
                                       ((upper >> SSB_INDEX & 1) == 0)) {
                if (LMSB == UMSB) {
                    result = result << 1 | LMSB;

                    if (++bits == WORD_BITS) {
                        dst.push_back(result);
                        bits = 0;
                        result = 0;
                    }

                    lower <<= 1;
                    upper = upper << 1 | 1;

                    while (Scale3 > 0) {
                        --Scale3;
                        result = result << 1 | LMSB ^ 1;

                        if (++bits == WORD_BITS) {
                            dst.push_back(result);
                            bits = 0;
                            result = 0;
                        }
                    }
                } else {
                    lower <<= 1;
                    upper = upper << 1 | 1;
                    lower ^= MSB_XOR;
                    upper ^= MSB_XOR;
                    ++Scale3;
                }

                LMSB = lower >> MSB_INDEX;
                UMSB = upper >> MSB_INDEX;
            }
        }

        size_t count = WORD_BITS;

        if (Scale3 > 0) {
            --count;
            size_t LMSB = lower >> MSB_INDEX;
            result <<= 1;
            result |= LMSB;

            if (++bits == WORD_BITS) {
                dst.push_back(result);
                bits = 0;
                result = 0;
            }

            LMSB ^= 1;

            while (Scale3--) {
                result <<= 1;
                result |= LMSB;

                if (++bits == WORD_BITS) {
                    dst.push_back(result);
                    bits = 0;
                    result = 0;
                }
            }
        }

        for (int i = count - 1; i >= 0; --i) {
            size_t LMSB = lower >> i & 1;
            result <<= 1;
            result |= LMSB;

            if (++bits == WORD_BITS) {
                dst.push_back(result);
                bits = 0;
                result = 0;
            }
        }

        size_t bitlen = bits + WORD_BITS * dst.size();

        if (bits > 0) {
            result <<= (WORD_BITS - bits);
            dst.push_back(result);
        }

        _encode_bit_len = bitlen;

        return dst;
    }

    std::vector<uint8_t> StatisticDecode(std::vector<value_type> src,
                                         size_t dst_size) {
        size_t encode_bit_len = _encode_bit_len;
        std::vector<uint8_t> dst;

        auto srcIt = src.begin();
        auto t = *srcIt;

        value_type readc = 0;
        encode_bit_len -= WORD_BITS;
        size_t bits_remain = 0;

        if (encode_bit_len > 0) {
            readc = *(++srcIt);
            bits_remain = WORD_BITS;
        }

        value_type lower = 0, upper = -1;

        while (true) {
            uint64_t range = (uint64_t)upper - lower + 1;
            count_t k = static_cast<count_t>(
                (((uint64_t)t - lower + 1) * _total_src_byte - 1) / range);
            auto it = this->find(table.begin(), table.end(), k);
            auto index = 0;

            if (it != table.end())
                index = std::distance(table.begin(), it);
            else
                index = MAX_TABLE_IDX;

            dst.push_back(index);

            value_type lower1 = static_cast<value_type>(
                lower +
                range * (index == 0 ? 0 : table[index - 1]) / _total_src_byte);
            value_type upper1 = static_cast<value_type>(
                lower + (range * table[index]) / _total_src_byte - 1);
            lower = lower1;
            upper = upper1;
            size_t LMSB = lower >> MSB_INDEX;
            size_t UMSB = upper >> MSB_INDEX;

            if (dst.size() == dst_size) return dst;

            while (LMSB == UMSB || (lower >> SSB_INDEX & 1) &&
                                       ((upper >> SSB_INDEX & 1) == 0)) {
                upper = upper << 1 | 1;
                lower <<= 1;
                t <<= 1;

                if (encode_bit_len > 0) {
                    if (bits_remain > 0) {
                        t = t | readc >> MSB_INDEX;
                        --bits_remain;
                        readc <<= 1;
                        --encode_bit_len;
                    }

                    if (bits_remain == 0 && encode_bit_len > 0) {
                        readc = *(++srcIt);
                        bits_remain = WORD_BITS;
                    }
                }

                if (LMSB != UMSB) {
                    lower ^= MSB_XOR;
                    upper ^= MSB_XOR;
                    t ^= MSB_XOR;
                }

                LMSB = lower >> MSB_INDEX;
                UMSB = upper >> MSB_INDEX;
            }
        }

        return dst;
    }

    std::vector<uint8_t> AdaptiveDecode(std::vector<value_type> src,
                                        size_t dst_size) {
        size_t encode_bit_len = _encode_bit_len;
        std::vector<uint8_t> dst;

        for (size_t i = 0; i < TABLE_SIZE; ++i) table[i] = i + 1;

        _total_src_byte = table[MAX_TABLE_IDX];

        auto srcIt = src.begin();
        auto t = *srcIt;
        value_type readc = 0;
        encode_bit_len -= WORD_BITS;
        size_t bits_remain = 0;

        if (encode_bit_len > 0) {
            readc = *(++srcIt);
            bits_remain = WORD_BITS;
        }

        value_type lower = 0, upper = -1;

        while (true) {
            uint64_t range = (uint64_t)upper - lower + 1;
            count_t k = static_cast<count_t>(
                (((uint64_t)t - lower + 1) * _total_src_byte - 1) / range);
            auto it = this->find(table.begin(), table.end(), k);
            auto index = 0;

            if (it != table.end())
                index = std::distance(table.begin(), it);
            else
                index = MAX_TABLE_IDX;

            dst.push_back(index);

            uint64_t lower1 = lower + range *
                                          (index == 0 ? 0 : table[index - 1]) /
                                          _total_src_byte;
            uint64_t upper1 =
                lower + range * table[index] / _total_src_byte - 1;
            lower = static_cast<value_type>(lower1);
            upper = static_cast<value_type>(upper1);
            size_t LMSB = lower >> MSB_INDEX;
            size_t UMSB = upper >> MSB_INDEX;

            if (dst.size() == dst_size) return dst;

            while (LMSB == UMSB || (lower >> SSB_INDEX & 1) &&
                                       ((upper >> SSB_INDEX & 1) == 0)) {
                upper = upper << 1 | 1;
                lower <<= 1;
                t <<= 1;

                if (encode_bit_len > 0) {
                    if (bits_remain > 0) {
                        t = t | readc >> MSB_INDEX;
                        --bits_remain;
                        readc <<= 1;
                        --encode_bit_len;
                    }

                    if (bits_remain == 0 && encode_bit_len > 0) {
                        readc = *(++srcIt);
                        bits_remain = WORD_BITS;
                    }
                }

                if (LMSB != UMSB) {
                    lower ^= MSB_XOR;
                    upper ^= MSB_XOR;
                    t ^= MSB_XOR;
                }

                LMSB = lower >> MSB_INDEX;
                UMSB = upper >> MSB_INDEX;
            }

            size_t temp = index;

            for (; temp < TABLE_SIZE; ++temp) ++table[temp];

            ++_total_src_byte;
        }

        return dst;
    }
};
}  // namespace Octinc

#endif
