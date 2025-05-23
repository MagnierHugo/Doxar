#pragma once

#include <vector>
#include <map>
#include <memory>
#include <mutex>


/**
 * \brief MemoryPool class (Singleton)
 */
class MemoryPool {

	/**
	 * \brief Used to lock the class and ensure thread safety 
	 */
	mutable std::mutex mtx;

public:
	/**
	 * \brief Handle and return singleton of MemoryPool class
	 * \return Instance of the MemoryPool
	 */
	static MemoryPool& Instance();

	/**
	 * \brief Asks the pool for a chunk of pre-allocated memory to use
	 * \param size -> Specify the size of the chunk of memory needed
	 * \return pointer towards a chunk of raw memory
	 */
	std::byte* Allocate(size_t size);
	/**
	 * \brief Return a chunk of memory previously allocated using MemoryPool::Allocate to the pool
	 * \param ptr -> Specify the start of the chunk to return to the pool
	 * \param size -> Specify the size of the chunk to return to the pool
	 */
	void Deallocate(std::byte* ptr, size_t size);
	/**
	 * \brief Allocate more memory to the pool
	 * \param size -> Specify the size of the additional memory to allocate
	 */
	void Extend(size_t size);

private:
	/**
	 * \brief Map of the different chunks available and there size
	 */
	std::map<std::byte*, size_t> pool;
	/**
	 * \brief Vector of original chunks of memory allocated at construction and by MemoryPool::Extend
	 * \brief Used to efficiently free the memory during destruction
	 */
	std::vector<std::byte*> allocPtr;
	/**
	 * \brief The pool total capacity
	 */
	size_t capacity;

	/**
	 * \brief Ctor doing the initial memory allocation
	 */
	MemoryPool();
	/**
	 * \brief Dtor freeing the allocated memory and clearing the map (pool) and vector (allocPtr)
	 */
	~MemoryPool();

	/**
	 * \brief Merge a chunk of memory with it's neighbours if possible to create one contiguous block
	 * \param it -> An iterator from pool specifying the chunk to try to merge with it's neighbours
	 */
	void MergeChunks(const std::map<std::byte*, size_t>::iterator& it);

public:
	/**
	 * \brief Delete copy ctor
	 */
	MemoryPool(const MemoryPool&) = delete;
	/**
	 * \brief Delete move ctor
	 */
	MemoryPool(MemoryPool&&) = delete;
	/**
	 * \brief Delete copy operator =
	 */
	MemoryPool& operator=(const MemoryPool&) = delete;
	/**
	 * \brief Delete move operator =
	 */
	MemoryPool& operator=(MemoryPool&&) = delete;

};