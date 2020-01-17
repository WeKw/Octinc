/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_AUTO_PTR__
#define OCTINC_AUTO_PTR__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <utility>

namespace Octinc {
template <typename __Tp>
class auto_ptr {
   private:
    __Tp* _ptr;
    bool _owner;

   public:
    auto_ptr(__Tp* _ptr = nullptr) : _ptr(_ptr), _owner(true) {}
    auto_ptr(auto_ptr& ap) : _ptr(ap._ptr), _owner(true) { ap._owner = false; }
    ~auto_ptr() {
        if (_owner) delete _ptr;
    }

    operator bool() const { return _owner; }

    auto_ptr operator=(__Tp* ptr) {
        _owner = true;
        _ptr = ptr;
    }

    auto_ptr operator=(auto_ptr& ap) {
        if (*this != ap) {
            if (_owner) delete _ptr;

            _ptr = ap._ptr;
            _owner = true;
            ap._owner = false;
        }

        return *this;
    }

    __Tp* operator->() { return _ptr; }

    __Tp& operator*() { return *_ptr; }

    void swap(auto_ptr& other) {
        std::swap(_ptr, other._ptr);
        std::swap(_owner, other._owner);
    }
};

class Delete {
   public:
    template <typename __Tp>
    void operator()(__Tp* p) const {
        delete p;
    }
};

template <typename __Tp, typename __Del = Delete>
class unique_ptr {
   private:
    __Tp* _ptr;
    __Del _del;

   public:
    explicit unique_ptr(__Tp* p = nullptr, const __Del& d = __Del())
        : _ptr(p), _del(d) {}
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& right_value)
        : _ptr(right_value._ptr), _del(std::move(right_value._del)) {
        right_value._ptr = nullptr;
    }

    ~unique_ptr() { _del(_ptr); }

    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr& operator=(unique_ptr&& right_value) noexcept {
        if (this != &right_value) {
            _del(*this);
            _ptr = right_value._ptr;
            _del = std::move(right_value._del);
            right_value._ptr = nullptr;
        }

        return *this;
    }

    __Tp* release() {
        __Tp* tmp = _ptr;
        _ptr = nullptr;
        return tmp;
    }

    void reset() { _del(_ptr); }

    void reset(__Tp* q) {
        if (_ptr) {
            _del(_ptr);
            _ptr = q;
        } else
            _ptr = nullptr;
    }

    void swap(unique_ptr& other) noexcept {
        std::swap(_ptr, other._ptr);
        std::swap(_del, other._del);
    }

    __Tp* get() { return _ptr; }

    __Del& get_deleter() { return _del; }

    __Tp& operator*() { return *_ptr; }

    __Tp* operator->() { return _ptr; }
};
}  // namespace Octinc

#endif
