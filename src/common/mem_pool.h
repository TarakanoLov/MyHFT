#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <common/macros.h>

namespace common {
  
template<typename T>
class MemPool final {
public:
    explicit MemPool(std::size_t num_elems) :
        store_(num_elems, {T(), true}) /* pre-allocation of vector storage. */ {
      ASSERT(reinterpret_cast<const ObjectBlock *>(&(store_[0].object_)) == &(store_[0]), "T object should be first member of ObjectBlock.");
    }

    template<typename... Args>
    T *allocate(Args... args) noexcept {
        updateNextFreeIndex();
        auto obj_block = &(store_[next_free_index_]);
        ASSERT(obj_block->is_free_, "Expected free ObjectBlock at index:" + std::to_string(next_free_index_));
        T *ret = &(obj_block->object_);
        *ret = T(args...);
        obj_block->is_free_ = false;

        return ret;
    }

    void deallocate(const T *elem) noexcept {
        const auto elem_index = (reinterpret_cast<const ObjectBlock *>(elem) - &store_[0]);
        ASSERT(elem_index >= 0 && static_cast<size_t>(elem_index) < store_.size(), "Element being deallocated does not belong to this Memory pool.");
        ASSERT(!store_[elem_index].is_free_, "Expected in-use ObjectBlock at index:" + std::to_string(elem_index));
        store_[elem_index].is_free_ = true;
    }
private:
    // It is better to have one vector of structs with two objects than two vectors of one object.
    // Consider how these are accessed and cache performance.
    struct ObjectBlock {
      T object_;
      bool is_free_ = true;
    };

public:
    std::pair<std::vector<ObjectBlock>, size_t> _only_for_tests_get_internals() const {
        return std::pair{store_, next_free_index_};
    }

    MemPool() = delete;

    MemPool(const MemPool &) = delete;

    MemPool(const MemPool &&) = delete;

    MemPool &operator=(const MemPool &) = delete;

    MemPool &operator=(const MemPool &&) = delete;

private:
    auto updateNextFreeIndex() noexcept {
        if (store_[next_free_index_].is_free_) [[unlikely]] {
            return ;
        }
        const auto initial_free_index = next_free_index_;
        while (!store_[next_free_index_].is_free_) {
            ++next_free_index_;
            if (next_free_index_ == store_.size()) [[unlikely]] {
                next_free_index_ = 0;
            }
            if (initial_free_index == next_free_index_) [[unlikely]] {
                ASSERT(initial_free_index != next_free_index_, "Memory Pool out of space.");
            }
      }
    }

    // We could've chosen to use a std::array that would allocate the memory on the stack instead of the heap.
    // We would have to measure to see which one yields better performance.
    // It is good to have objects on the stack but performance starts getting worse as the size of the pool increases.
    std::vector<ObjectBlock> store_;

    size_t next_free_index_ = 0;
  };
}