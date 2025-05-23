#pragma once

#include <typeindex>

#include "Utilities/MemoryPool.hpp"
#include "Utilities/Logger.hpp"


#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define ARGS_PROCESSING(macro, ...)									\
  __VA_OPT__(EXPAND(ARGS_PROCESSING_HELPER(macro, __VA_ARGS__)))

#define ARGS_PROCESSING_HELPER(macro, a1, ...)						\
  macro(a1)															\
  __VA_OPT__(, ARGS_PROCESSING_AGAIN PARENS (macro, __VA_ARGS__))

#define ARGS_PROCESSING_AGAIN() ARGS_PROCESSING_HELPER

template<typename T>
concept _IsPointer2 = std::is_pointer<T>::value && !(std::is_same<T, char*>::value || std::is_same<T, const char*>::value);

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(_IsPointer2<T>)
static constexpr const void* log_pointer_cast2(T ptr) {
    return static_cast<void*>(ptr);
}

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(!_IsPointer2<T>)
static constexpr const T log_pointer_cast2(T& value) {
    return std::forward<T>(value);
}

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(!_IsPointer2<T>)
static constexpr const T log_pointer_cast2(T&& value) {
    return std::forward<T>(value);
}

#define PROCESS_ARG2(arg) log_pointer_cast2<decltype(arg)>(arg)

#define LOG_DEBUG2(message, ...)    ::Logger::GetLogger()->debug(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG2, __VA_ARGS__)))

template<typename T>
T* PoolAllocator<T>::allocate(size_t n) {
    LOG_DEBUG2("Pool Allocator Allocating {} elements of type: {}", n, typeid(T).name());
    if (n > std::allocator_traits<PoolAllocator>::max_size(*this)) {
        throw std::bad_alloc();
    }
    return reinterpret_cast<T*>(MemoryPool::Instance().Allocate(n * sizeof(T)));
}

template<typename T>
void PoolAllocator<T>::deallocate(T* ptr, size_t n) noexcept {
    LOG_DEBUG2("Pool Allocator Deallocating {} elements of type: {} at {}", n, typeid(T).name(), ptr);
    MemoryPool::Instance().Deallocate(reinterpret_cast<std::byte*>(ptr), n * sizeof(T));
}

template<typename T>
template<typename U, typename... Args>
void PoolAllocator<T>::construct(U* ptr, Args&&... args) {
    LOG_DEBUG2("Pool Allocator Construction of an element of type: {} at {}", typeid(U).name(), ptr);
    new (ptr) U(std::forward<Args>(args)...);
}

template<typename T>
template<typename U>
void PoolAllocator<T>::destroy(U* ptr) noexcept {
    LOG_DEBUG2("Pool Allocator Destruction of an element of type: {} at {}", typeid(U).name(), ptr);
    ptr->~U();
}