#pragma once

class Discarder {
public:
	template<typename T> constexpr inline void operator=(const T&) const { }
private:
};

inline constexpr const Discarder _;

#define Nameof(sth) #sth