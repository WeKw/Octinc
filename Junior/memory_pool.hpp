/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_MEMORY_POOL__
#define OCTINC_MEMORY_POOL__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstring>
#include <vector>

namespace Octinc {
template <typename __Tp, size_t block_size = 4096>
class memory_pool {
   private:
    union slot {
        __Tp element;
        slot* next;
    };

    typedef char* data_pointer;
    typedef slot slot_type;
    typedef slot* slot_pointer;

    slot_pointer current_block, current_slot, last_slot, free_slots;
    static_assert(block_size >= 2 * sizeof(slot_type),
                  "MEMORY_POOL:BlockSize too small.");

   public:
    typedef __Tp* pointer;

    template <typename __Tp2>
    struct rebind {
        typedef memory_pool<__Tp2> other;
    };

    inline memory_pool() noexcept {
        current_block = nullptr;
        current_slot = nullptr;
        last_slot = nullptr;
        free_slots = nullptr;
    }

    inline ~memory_pool() noexcept {
        slot_pointer curr = current_block;

        while (curr != nullptr) {
            slot_pointer prev = curr->next;
            operator delete(reinterpret_cast<void*>(curr));
            curr = prev;
        }
    }

    inline pointer allocate(size_t n = 1, const __Tp* hint = nullptr)
    /*
    Apply for memory and return pointers.
    */
    {
        if (free_slots != nullptr) {
            pointer result = reinterpret_cast<pointer>(free_slots);
            free_slots = free_slots->next;
            return result;
        } else {
            if (current_slot >= last_slot) {
                data_pointer new_block =
                    reinterpret_cast<data_pointer>(operator new(block_size));
                reinterpret_cast<slot_pointer>(new_block)->next = current_block;
                current_block = reinterpret_cast<slot_pointer>(new_block);
                data_pointer body = new_block + sizeof(slot_pointer);
                uintptr_t result = reinterpret_cast<uintptr_t>(body);
                size_t body_padding =
                    (alignof(slot_type) - result) % alignof(slot_type);
                current_slot =
                    reinterpret_cast<slot_pointer>(body + body_padding);
                last_slot = reinterpret_cast<slot_pointer>(
                    new_block + block_size - sizeof(slot_type));
            }

            return reinterpret_cast<pointer>(current_slot++);
        }
    }

    inline void deallocate(pointer p, size_t n = 1)
    /*
    Free up memory.
    */
    {
        if (p != nullptr) {
            reinterpret_cast<slot_pointer>(p)->next = free_slots;
            free_slots = reinterpret_cast<slot_pointer>(p);
        }
    }

    template <typename __Tp2, typename... Args>
    inline void construct(__Tp2* p, Args&&... args) {
        new (p) __Tp2(std::forward<Args>(args)...);
    }

    template <typename __Tp2>
    inline void destroy(__Tp2* p) {
        p->~__Tp2();
    }
};

template <typename __Tp, const int block_size = 10000000>
class memory_allocator : std::allocator<__Tp> {
   private:
    char alloc_pool[block_size], *alloc_pointer;

   public:
    memory_allocator() {
        memset(alloc_pool, 0, sizeof(alloc_pool));
        *alloc_pointer = alloc_pool;
    }

    template <typename __Tp2>
    memory_allocator(const memory_allocator<__Tp2>& alloc_) {}

    template <typename __Tp2>
    memory_allocator<__Tp>& operator=(const memory_allocator<__Tp2>& alloc_) {
        return *this;
    }

    template <typename __Tp2>
    struct rebind {
        typedef memory_allocator<__Tp2> other;
    };

    inline __Tp* allocate(size_t n) {
        __Tp* result = (__Tp*)alloc_pointer;
        alloc_pointer += n * sizeof(__Tp);
        return result;
    }

    inline void deallocate(__Tp* p, size_t n) {}
};
}  // namespace Octinc

#endif
