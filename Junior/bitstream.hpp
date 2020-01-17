/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_BITSTREAM__
#define OCTINC_BITSTREAM__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace Octinc {
typedef uint8_t BIT;

template <size_t buff_size>
class bitstream {
   private:
    FILE* _file;
    BIT _readbuf[buff_size];
    BIT _writebuf[buff_size];
    uint16_t _read_offset;
    uint8_t _read_bit_offset;
    uint16_t _read_buff_size;
    uint8_t _write_bit_offset;
    uint16_t _write_buff_size;
    static const int _buff_size = buff_size;

   protected:
    size_t write_internal() {
        if (_file && _write_buff_size > 0)
            return fwrite(_writebuf, 1, _write_buff_size, _file);

        return 0;
    }

    size_t read_internal() {
        if (_file && _read_buff_size == 0)
            _read_buff_size = fread(_readbuf, 1, buff_size, _file);

        return _read_buff_size;
    }

   public:
    bitstream() {
        _file = nullptr;
        _read_offset = 0;
        _read_bit_offset = 0;
        _read_buff_size = 0;
        _write_bit_offset = 0;
        _write_buff_size = 0;
    }
    ~bitstream() { close(); }

    void open(const char* filename, const char* mode)
    /*
    Open the file in the specified format.
    */
    {
        close();
        _file = fopen(filename, mode);
    }

    void close()
    /*
    Close the file.
    */
    {
        if (!_file) return;

        while (_write_bit_offset > 0) {
            if (++_write_bit_offset == 8) {
                ++_write_buff_size;
                _write_bit_offset = 0;
            }

            if (_write_buff_size >= buff_size)
                if (write_internal() != _write_buff_size) _write_buff_size = 0;
        }

        write_internal();
        fflush(_file);
        fclose(_file);
        _file = nullptr;
    }

    BIT operator()()
    /*
    Read a bit from the file.
    */
    {
        if (_read_buff_size == 0)
            if (read_internal() == 0) {
                fprintf(stderr, "BITSTREAM:Read to EOF.");
                return 0;
            }

        BIT bit = (_readbuf[_read_offset] >> (7 - _read_bit_offset) & 1);

        if (++_read_bit_offset == 8) {
            ++_read_offset;
            _read_bit_offset = 0;
        }

        if (_read_offset >= _read_buff_size) _read_buff_size = 0;

        return bit;
    }

    friend bitstream& operator<<(bitstream& bs, BIT bit)
    /*
    Write a bit into the file.
    */
    {
        assert(bit == 0 || bit == 1);
        bs._writebuf[bs._write_buff_size] =
            (bs._writebuf[bs._write_buff_size] << 1) | bit;

        if (++bs._write_bit_offset == 8) {
            ++bs._write_buff_size;
            bs._write_bit_offset = 0;
        }

        if (bs._write_buff_size >= bitstream::_buff_size)
            if (bs.write_internal() != bs._write_buff_size) {
                fprintf(stderr, "BITSTREAM:Write error.");
                bs._write_buff_size = 0;
            }

        return bs;
    }

    size_t size()
    /*
    Get the size of file.
    */
    {
        if (!_file) return 0;

        fseek(_file, 0L, SEEK_END);
        long siz = ftell(_file);
        fseek(_file, 0L, SEEK_SET);
        return static_cast<size_t>(siz * 8);
    }
};

typedef bitstream<1024> Bitstream;  // Efficiency optimization
}  // namespace Octinc

#endif
