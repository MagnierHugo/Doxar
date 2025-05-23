#pragma once


/**
 * \brief PoolAllocator<T> class (Custom Allocator)
 */
template<typename T>
class PoolAllocator {
public:
	/**
	 * \brief Type alias for compatibility with the STL containers
	 */
	using value_type = T;

	/**
	 * \brief Default ctor
	 */
	PoolAllocator() = default;
	/**
	 * \brief Default dtor
	 */
	~PoolAllocator() = default;

	/**
	 * \brief Templated copy ctor to allow copying between different types of allocators
	 */
	template<typename U>
	constexpr PoolAllocator(const PoolAllocator<U>&) noexcept {}

	/**
	 * \brief Request a chunk of memory from the Memory Pool
	 * \param n -> The number of element of type T to allocate
	 * \return Returns a pointer of type T to the requested memory
	 */
	T* allocate(size_t n);
	/**
	 * \brief Return the memory to the memory pool
	 * \param ptr -> The pointer of type T to the start of the chunk to return
	 * \param n -> The number of elements to deallocate
	 */
	void deallocate(T* ptr, size_t n) noexcept;

	/**
	 * \brief Constructs an element of type U at the specified address
	 * \param ptr -> The pointer of type U to the construction address
	 * \param args -> The arguments to pass to the ctor of type U
	 */
	template<typename U, typename... Args>
	void construct(U* ptr, Args&&... args);

	/**
	 * \brief Destroys an element of type U
	 * \param ptr -> The pointer of type U to the element to destroy
	 */
	template<typename U>
	void destroy(U* ptr) noexcept;

	/**
	 * \brief operator ==, defined as friend for STL compatibility
	 */
	friend bool operator==(const PoolAllocator&, const PoolAllocator&) { return true; }
	/**
	 * \brief operator !=, defined as friend for STL compatibility
	 */
	friend bool operator!=(const PoolAllocator&, const PoolAllocator&) { return false; }

private:
};

#include "PoolAllocator.inl"