#include "Utilities/MemoryPool.hpp"

#include <iostream>
#include "Utilities/Logger.hpp"


MemoryPool::MemoryPool() {
	LOG_DEBUG("Memory Pool Instance Created");
	capacity = 1'048'576; // 1 MiB (mebibyte) the initial pool capacity
	std::byte* ptr = reinterpret_cast<std::byte*>(operator new(capacity));
	allocPtr.push_back(ptr);
	pool.emplace(ptr, capacity);
}

MemoryPool::~MemoryPool() {
	LOG_DEBUG("Memory Pool Instance Destroyed");

	for (std::byte* ptr : allocPtr) {
		operator delete(ptr);
	}

	pool.clear();
	allocPtr.clear();
}


MemoryPool& MemoryPool::Instance() {
	static MemoryPool instance;
	return instance;
}

std::byte* MemoryPool::Allocate(size_t size) {

	{ // artificial scope for the mutex because flemme
		std::scoped_lock lock(mtx);
		std::byte* ptr;

		for (auto& [key, value] : pool) {
			if (size > value) { continue; }

			ptr = key;

			if (size < value) {
				pool.emplace(key + size, value - size);
			}

			pool.erase(key);
			LOG_DEBUG("Memory Pool Allocation of {} bytes at: {}", size, ptr);
			return ptr;
		}

		size < capacity ? Extend(capacity) : Extend(size);
	}

	return Allocate(size);
}

void MemoryPool::Deallocate(std::byte* ptr, size_t size) {
	LOG_DEBUG("Memory Pool Deallocation of pointer: {} of size: {}", ptr, size);

	{ // artificial scope for the mutex because flemme
		std::scoped_lock lock(mtx);

		pool.emplace(ptr, size);
	}

	MergeChunks(pool.find(ptr));
}

void MemoryPool::Extend(size_t size) {
	LOG_DEBUG("Memory Pool Extention of {} bytes", size);
	std::byte* ptr;

	{ // artificial scope for the mutex because flemme
		std::scoped_lock lock(mtx);
		ptr = reinterpret_cast<std::byte*>(operator new(size));

		allocPtr.push_back(ptr);
		pool.emplace(ptr, size);
		capacity += size;
	}

	MergeChunks(pool.find(ptr));
}

void MemoryPool::MergeChunks(const std::map<std::byte*, size_t>::iterator& it) {
	std::scoped_lock lock(mtx);

	if (it == pool.end()) { return; }

	const auto& next = std::next(it);
	if (next != pool.end()) {
		if (it->first + it->second == next->first) {
			LOG_DEBUG("Memory Pool Merging chunks {} of size: {} with {} of size: {}", it->first, it->second, next->first, next->second);
			it->second += next->second;
			LOG_DEBUG("Resulting chunk {} of size: {}", it->first, it->second);
			pool.erase(next);
		}
	}

	if (it == pool.begin()) { return; }

	const auto& previous = std::prev(it);
	if (previous->first + previous->second == it->first) {
		LOG_DEBUG("Memory Pool Merging chunks {} of size: {} with {} of size: {}", previous->first, previous->second, it->first, it->second);
		previous->second += it->second;
		LOG_DEBUG("Resulting chunk {} of size: {}", previous->first, previous->second);
		pool.erase(it);
	}
}