#pragma once


template<typename T>
requires(IsDefaultConstructible<T>)
const PoolAllocator<T> ObjectPooling<T>::allocator;


template<typename T>
requires(IsDefaultConstructible<T>)
ObjectPooling<T>::ObjectPooling(const size_t& n) {
	pool.reserve(n);

	for (size_t i{}; i < n; i++) {
		pool.push_back(std::allocate_shared<T>(allocator));
	}
}

template<typename T>
requires(IsDefaultConstructible<T>)
ObjectPooling<T>::~ObjectPooling() {
	pool.clear();
}

template<typename T>
requires(IsDefaultConstructible<T>)
std::shared_ptr<T> ObjectPooling<T>::Get() {

	if (pool.empty()) {
		return std::allocate_shared<T>(allocator);
	}

	size_t index = pool.size() - 1;
	std::shared_ptr<T> ptr{ std::move(pool[index]) };
	pool.erase(pool.begin() + index);

	return ptr;
}

template<typename T>
requires(IsDefaultConstructible<T>)
void ObjectPooling<T>::Return(std::shared_ptr<T>&& ptr) {
	pool.push_back(std::move(ptr));
}