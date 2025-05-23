#pragma once

#include <vector>
#include "Utilities/PoolAllocator.hpp"


template<typename T> using PoolVector = std::vector<T, PoolAllocator<T>>;


#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__


/**
 * \brief Pass each variadic args throught a macro
 * \brief ARGS_PROCESSING(F, a, b, c, d) -> F(a) F(b) F(c) F(d)
 * \param macro -> macro called on the args
 * \param ... -> every variadic args
 */
#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))

#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))

#define FOR_EACH_AGAIN() FOR_EACH_HELPER


#define ENUM_CASE(name) case name: return #name;

#define MAKE_ENUM(type, ...)				\
  enum class type {							\
	  __VA_ARGS__							\
  };										\
											\
  constexpr const char*						\
  to_string(type _e) {						\
	  using enum class type;				\
	  switch (_e) {							\
	  FOR_EACH(ENUM_CASE, __VA_ARGS__)		\
	  default:								\
		  return "unknown";					\
	  }										\
  }


/**
 * \brief Pass each variadic args throught a macro
 * \brief ARGS_PROCESSING(F, a, b, c, d) -> F(a), F(b), F(c), F(d)
 * \param macro -> macro called on the args
 * \param ... -> every variadic args
 */
#define ARGS_PROCESSING(macro, ...)									\
  __VA_OPT__(EXPAND(ARGS_PROCESSING_HELPER(macro, __VA_ARGS__)))

#define ARGS_PROCESSING_HELPER(macro, a1, ...)						\
  macro(a1)															\
  __VA_OPT__(, ARGS_PROCESSING_AGAIN PARENS (macro, __VA_ARGS__))

#define ARGS_PROCESSING_AGAIN() ARGS_PROCESSING_HELPER