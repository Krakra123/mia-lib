// TODO Implement dynamic arena

#ifndef MIA_ARENA_HPP_
#define MIA_ARENA_HPP_

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <utility>

#ifndef ARENA_DEFAULT_CAPACITY
#define ARENA_DEFAULT_CAPACITY 4 * 1024
#endif  // !ARENA_DEFAULT_CAPACITY

namespace mia {

struct arena {
    char* buffer;
    size_t curoffset;
    size_t capacity;

    arena(size_t init_capacity = 0) {
        capacity = init_capacity;
        if (init_capacity == 0) {
            capacity = ARENA_DEFAULT_CAPACITY;
        }

        buffer = (char*)malloc(capacity);
        curoffset = 0;
    }

    ~arena() { free(buffer); }

    arena(const arena& other) = delete;
    arena& operator=(const arena& other) = delete;

    arena(arena&& other) noexcept {
        buffer = std::exchange(other.buffer, nullptr);
        curoffset = std::exchange(other.curoffset, 0);
        capacity = std::exchange(other.capacity, 0);
    }
    arena& operator=(arena&& other) noexcept {
        if (this != &other) {
            delete[] buffer;

            buffer = std::exchange(other.buffer, nullptr);
            curoffset = std::exchange(other.curoffset, 0);
            capacity = std::exchange(other.capacity, 0);
        }
        return *this;
    }

    template <typename T, class Allocator = std::allocator<T>, typename... Args>
    inline T* alloc(Args&&... args) {
        size_t size = sizeof(T);
        size_t align = alignof(T);

        uintptr_t alloc_res = (uintptr_t)buffer + curoffset;
        uintptr_t padding = (~alloc_res + 1) & (align - 1);
        alloc_res += padding;

        curoffset += size + padding;

        Allocator alloc;
        using AllocTraits = std::allocator_traits<Allocator>;

        T* res_ptr = reinterpret_cast<T*>(alloc_res);
        AllocTraits::construct(alloc, res_ptr, std::forward<Args>(args)...);
        return res_ptr;
    }

    template <typename T, class Allocator = std::allocator<T>>
    inline T* add(T&& other) noexcept {
        size_t size = sizeof(T);
        size_t align = alignof(T);

        uintptr_t alloc_res = (uintptr_t)buffer + curoffset;
        uintptr_t padding = (~alloc_res + 1) & (align - 1);
        alloc_res += padding;

        curoffset += size + padding;

        Allocator alloc;
        using AllocTraits = std::allocator_traits<Allocator>;

        T* res_ptr = reinterpret_cast<T*>(alloc_res);
        AllocTraits::construct(alloc, res_ptr, other);
        return res_ptr;
    }
};

}  // namespace mia

#endif  // !ARENA_HPP_
