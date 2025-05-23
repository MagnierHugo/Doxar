#pragma once

#include <vector>
#include <memory>

#include "PoolAllocator.hpp"


/**
 * \brief Requires a template type to have a default ctor
 */
template<typename T>
concept IsDefaultConstructible = requires {
	T{};
};


/**
 * \brief ObjectPooling<T> class
 */
template<typename T>
requires(IsDefaultConstructible<T>)
class ObjectPooling {
public:
	/**
	 * \brief Default ctor
	 */
	ObjectPooling() = default;
	/**
	 * \brief Ctor pre-constructing a fix amount of objects
	 * \param n -> The number of objects to pre-create
	 */
	ObjectPooling(const size_t& n);
	/**
	 * \brief Dtor clearing the object pool
	 */
	~ObjectPooling();

	/**
	 * \brief Requests an Object from the pool
	 * \return std::shared_ptr of type T towards the requested object
	 */
	std::shared_ptr<T> Get();
	/**
	 * \brief Give a requested Object back to the pool
	 * \brief Invalidate the returned object
	 * \param ptr -> std::shared_ptr of type T towards the object to return
	 */
	void Return(std::shared_ptr<T>&& ptr);

private:
	/**
	 * \brief Static allocator for the use of std::allocate_shared
	 */
	static const PoolAllocator<T> allocator;

	/**
	 * \brief Vector of Ts using the custom allocator class PoolAllocator<T>
	 */
	std::vector<std::shared_ptr<T>, PoolAllocator<std::shared_ptr<T>>> pool;

};

#include "ObjectPooling.inl"