/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_POINTER_VECTOR__
#define OCTINC_POINTER_VECTOR__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <malloc.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <random>

namespace Octinc {
template <typename __Tp>
class pointer_vector {
   private:
    virtual void shrink() {
        if (_cap < (_size << 2) || _cap <= MIN_items) return;

        __Tp* olditems = items;
        items = new __Tp[_cap >>= 1];

        for (uint32_t i = 0; i < _size; ++i) items[i] = olditems[i];

        delete[] olditems;
    }

    virtual void expand() {
        if (_cap > _size) return;

        __Tp* olditems = items;
        items = new __Tp[_cap <<= 1];

        for (uint32_t i = 0; i < _size; ++i) items[i] = olditems[i];

        delete[] olditems;
    }

    template <typename __Cmp>
    __Tp* vct_make_axle(__Tp* begp, __Tp* endp) {
        std::mt19937 rnd(endp - begp);
        __Tp* randp = begp + (rnd() % (endp - begp));
        __Tp frog = *randp;
        *randp = *begp;
        __Cmp cmp;

        while (true) {
            while (cmp(frog, *(--endp)))
                ;

            if (begp < endp)
                *begp = *endp;
            else {
                *begp = frog;
                return begp;
            }

            while (begp < endp && cmp(*(++begp), frog))
                ;

            if (begp < endp)
                *endp = *begp;
            else {
                *endp = frog;
                return endp;
            }
        }
    }

    template <typename __Cmp>
    void vct_qsort(__Tp* begp, __Tp* endp) {
        if (endp - begp < 2) return;

        __Tp* ax = vct_make_axle<__Cmp>(begp, endp);
        vct_qsort<__Cmp>(begp, ax);
        vct_qsort<__Cmp>(ax + 1, endp);
    }

   protected:
    __Tp* items;
    size_t _size;
    uint32_t _cap;
    static const uint32_t MIN_items = 8;

   public:
    pointer_vector() : _size(0) { items = new __Tp[_cap = MIN_items]; }

    pointer_vector(const __Tp* arrt, const uint32_t& _len) {
        _size = _cap = _len;
        items = new __Tp[_cap];

        for (uint32_t i = 0; i < _size; ++i) items[i] = arrt[i];
    }

    pointer_vector(const pointer_vector& other) { *this = other; }

    ~pointer_vector() { delete[] items; }

    __Tp& operator[](const uint32_t rank) { return items[rank]; }

    pointer_vector<__Tp>& operator=(const pointer_vector& vec) {
        _size = vec._size;
        items = new __Tp[_cap = vec._cap];

        for (uint32_t i = 0; i < _size; ++i) items[i] = vec.items[i];

        return *this;
    }

    void push_back(const __Tp& v) {
        items[_size++] = v;
        expand();
    }

    __Tp pop_back() {
        __Tp res = items[--_size];
        shrink();
        return res;
    }

    virtual size_t size() { return _size; }

    virtual bool empty() { return !_size; }

    virtual void clear() {
        _size = 0;
        delete[] items;
        items = new __Tp[_cap = MIN_items];
    }

    __Tp* begin()
    /*
    Return the pointer of beginning address.
    */
    {
        return items;
    }

    __Tp* end()
    /*
    Return the pointer of end address.
    */
    {
        return (items + _size);
    }

    pointer_vector<__Tp>& swap(pointer_vector& vct)
    /*
    Swap two vectors.
    */
    {
        __Tp* olditems = this->items;
        this->items = vct.items;
        vct.items = olditems;

        size_t oldsize = this->_size;
        this->_size = vct._size;
        vct._size = oldsize;

        uint32_t oldcap = this->_cap;
        this->_cap = vct._cap;
        vct._cap = oldcap;

        return *this;
    }

    template <typename __Cmp = std::less<int> >
    pointer_vector<__Tp>& sort()
    /*
    Sort the vector with comparing class.
    You can call this function as
    vec.sort() or vec.sort<cmp>().
    */
    {
        vct_qsort<__Cmp>(items, items + _size);
        return *this;
    }

    pointer_vector<__Tp>& unique()
    /*
    Unique the vector.
    */
    {
        __Tp* olditems = items;
        items = new __Tp[_cap];

        uint32_t j = 0;
        items[0] = olditems[0];

        for (uint32_t i = 0; i < _size; ++i)
            if (!(olditems[i] == items[j])) items[++j] = olditems[i];

        _size = j + 1;
        delete[] olditems;
        shrink();
        return *this;
    }
};

#define real_size (_size - ex_sz)

template <typename __Tp>
class pointer_stack : private pointer_vector<__Tp> {
    using pointer_vector<__Tp>::_size;
    using pointer_vector<__Tp>::_cap;
    using pointer_vector<__Tp>::items;
    using pointer_vector<__Tp>::MIN_items;

   private:
    void shrink() {
        if (_cap < (_size << 2) || _cap <= MIN_items) return;

        __Tp* olditems = items;
        items = new __Tp[_cap >>= 1];

        for (uint32_t i = 0; i < _size; ++i) items[i] = olditems[i];

        delete[] olditems;
    }

    void expand() {
        if (_cap > _size) return;

        __Tp* olditems = items;
        items = new __Tp[_cap <<= 1];

        for (uint32_t i = 0; i < _size; ++i) items[i] = olditems[i];

        delete[] olditems;
    }

   public:
    pointer_stack() {}
    pointer_stack(const pointer_stack& other) { *this = other; }

    void push(__Tp v)
    /*
    Push an element into stack.
    */
    {
        items[_size++] = v;
        expand();
    }

    __Tp pop()
    /*
    Delete and return the top of the stack.
    */
    {
        __Tp res = items[--_size];
        shrink();
        return res;
    }

    __Tp& top()
    /*
    Return the top of the stack.
    */
    {
        return items[_size - 1];
    }

    size_t size() { return _size; }

    bool empty() { return !_size; }

    void clear() {
        _size = 0;
        delete[] items;
        items = new __Tp[_cap = MIN_items];
    }
};

template <typename __Tp>
class pointer_queue : private pointer_vector<__Tp> {
    using pointer_vector<__Tp>::_size;
    using pointer_vector<__Tp>::_cap;
    using pointer_vector<__Tp>::items;
    using pointer_vector<__Tp>::MIN_items;

   private:
    size_t ex_sz;

    void shrink() {
        if (_cap < (real_size << 2) || _cap <= MIN_items) return;

        __Tp* olditems = items;
        items = new __Tp[_cap >>= 1];

        for (uint32_t i = ex_sz; i < _size; ++i) items[i - ex_sz] = olditems[i];

        delete[] olditems;
        _size -= ex_sz;
        ex_sz = 0;
    }

    void expand() {
        if (_cap > _size) return;

        __Tp* olditems = items;
        items = new __Tp[_cap <<= 1];

        for (uint32_t i = ex_sz; i < _size; ++i) items[i - ex_sz] = olditems[i];

        delete[] olditems;
        _size -= ex_sz;
        ex_sz = 0;
    }

   public:
    pointer_queue() : ex_sz(0) {}
    pointer_queue(const pointer_queue& other) : ex_sz(0) { *this = other; }

    void push(__Tp v)
    /*
    Push an element into queue.
    */
    {
        items[_size++] = v;
        expand();
    }

    __Tp pop_front()
    /*
    Delete and return the front of the queue.
    */
    {
        __Tp res = items[ex_sz++];
        shrink();
        return res;
    }

    __Tp pop_back()
    /*
    Delete and return the back of the queue.
    */
    {
        __Tp res = items[--_size];
        shrink();
        return res;
    }

    __Tp& front() { return items[ex_sz]; }

    __Tp& back() { return items[_size - 1]; }

    size_t size() { return real_size; }

    bool empty() { return !real_size; }

    void clear() {
        _size = 0;
        ex_sz = 0;
        delete[] items;
        items = new __Tp[_cap = MIN_items];
    }
};

#undef real_size
}  // namespace Octinc

#endif
