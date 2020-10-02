// KT header-only library
// Requirements: C++17

#pragma once
#include <algorithm>
#include <bitset>
#include <type_traits>

namespace kt {
///
/// \brief Type-safe and index-safe wrapper for `std::bitset`
/// \param `Enum`: enum [class] type
/// \param `N`: number of bits (defaults to Enum::eCOUNT_)
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
struct enum_flags {
	static_assert(std::is_enum_v<Enum>, "Enum must be an enum!");

	using type = Enum;
	static constexpr std::size_t size = N;

	std::bitset<N> bits;

	static constexpr enum_flags<Enum, N> inverse() noexcept;

	constexpr enum_flags() noexcept = default;
	///
	/// \brief Construct with a single flag set
	///
	constexpr /*implicit*/ enum_flags(Enum flag) noexcept;

	///
	/// \brief Check if passed flags are set
	///
	constexpr bool test(enum_flags<Enum, N> flags) const noexcept;
	///
	/// \brief Obtain count of set flags
	///
	constexpr std::size_t test() const noexcept;
	///
	/// \brief Set passed flags
	///
	constexpr enum_flags<Enum, N>& set(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Reset passed flags
	///
	constexpr enum_flags<Enum, N>& reset(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Flip passed flags
	///
	constexpr enum_flags<Enum, N>& flip(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Set all flags
	///
	constexpr enum_flags<Enum, N>& set() noexcept;
	///
	/// \brief Reset all flags
	///
	constexpr enum_flags<Enum, N>& reset() noexcept;
	//
	/// \brief Flip all flags
	///
	constexpr enum_flags<Enum, N>& flip() noexcept;
	///
	/// \brief Check if all passed flags are set
	///
	constexpr bool all(enum_flags<Enum, N> flags = inverse()) const noexcept;
	///
	/// \brief Check if any passed flags are set
	///
	constexpr bool any(enum_flags<Enum, N> flags = inverse()) const noexcept;
	///
	/// \brief Check if no passed flags are set
	///
	constexpr bool none(enum_flags<Enum, N> flags = inverse()) const noexcept;
	///
	/// \brief Obtain count of set flags
	///
	constexpr std::size_t count(enum_flags<Enum, N> flags = inverse()) const noexcept;

	///
	/// \brief Obtain a reference to the flag
	///
	constexpr typename std::bitset<N>::reference operator[](Enum flag) noexcept;
	///
	/// \brief Perform bitwise `OR` / add flags
	///
	constexpr enum_flags<Enum, N>& operator|=(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Perform bitwise `AND` / multiply flags
	///
	constexpr enum_flags<Enum, N>& operator&=(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Perform bitwise `XOR` / exclusively add flags (add mod 2)
	///
	constexpr enum_flags<Enum, N>& operator^=(enum_flags<Enum, N> flags) noexcept;
	///
	/// \brief Perform bitwise `OR` / add flags
	///
	constexpr enum_flags<Enum, N> operator|(enum_flags<Enum, N> flags) const noexcept;
	///
	/// \brief Perform bitwise `AND` / multiply flags
	///
	constexpr enum_flags<Enum, N> operator&(enum_flags<Enum, N> flags) const noexcept;
	///
	/// \brief Perform bitwise `XOR` / exclusively add flags (add mod 2)
	///
	constexpr enum_flags<Enum, N> operator^(enum_flags<Enum, N> flags) const noexcept;
};
///
/// \brief Test if flags are equal
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
constexpr bool operator==(enum_flags<Enum, N> lhs, enum_flags<Enum, N> rhs) noexcept;
///
/// \brief Test if flags are not equal
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
constexpr bool operator!=(enum_flags<Enum, N> lhs, enum_flags<Enum, N> rhs) noexcept;
} // namespace kt

///
/// \brief Add two flags
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
constexpr kt::enum_flags<Enum, N> operator|(Enum flag1, Enum flag2) noexcept;
///
/// \brief Multiply two flags
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
constexpr kt::enum_flags<Enum, N> operator&(Enum flag1, Enum flag2) noexcept;
///
/// \brief Exclusively add two flags
///
template <typename Enum, std::size_t N = (std::size_t)Enum::eCOUNT_>
constexpr kt::enum_flags<Enum, N> operator^(Enum flag1, Enum flag2) noexcept;

namespace kt {
template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N> enum_flags<Enum, N>::inverse() noexcept {
	return enum_flags<Enum, N>().flip();
}

template <typename Enum, std::size_t N>
constexpr /*implicit*/ enum_flags<Enum, N>::enum_flags(Enum flag) noexcept {
	bits.set((std::size_t)flag);
}

template <typename Enum, std::size_t N>
constexpr bool enum_flags<Enum, N>::test(enum_flags<Enum, N> flags) const noexcept {
	return (bits & flags.bits) == flags.bits;
}

template <typename Enum, std::size_t N>
constexpr std::size_t enum_flags<Enum, N>::test() const noexcept {
	return bits.count();
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::set(enum_flags<Enum, N> flags) noexcept {
	bits |= flags.bits;
	return *this;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::reset(enum_flags<Enum, N> flags) noexcept {
	bits &= flags.bits.flip();
	return *this;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::flip(enum_flags<Enum, N> flags) noexcept {
	bits ^= flags.bits;
	return *this;
}

template <typename Enum, std::size_t N>
constexpr bool enum_flags<Enum, N>::all(enum_flags<Enum, N> flags) const noexcept {
	return (flags.bits & bits) == bits;
}

template <typename Enum, std::size_t N>
constexpr bool enum_flags<Enum, N>::any(enum_flags<Enum, N> flags) const noexcept {
	return (flags.bits & bits).any();
}

template <typename Enum, std::size_t N>
constexpr bool enum_flags<Enum, N>::none(enum_flags<Enum, N> flags) const noexcept {
	return (flags.bits & bits).none();
}

template <typename Enum, std::size_t N>
constexpr std::size_t enum_flags<Enum, N>::count(enum_flags<Enum, N> flags) const noexcept {
	return (flags.bits & bits).count();
}

template <typename Enum, std::size_t N>
constexpr typename std::bitset<N>::reference enum_flags<Enum, N>::operator[](Enum flag) noexcept {
	return bits[(std::size_t)flag];
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::operator|=(enum_flags<Enum, N> flags) noexcept {
	bits |= flags.bits;
	return *this;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::operator&=(enum_flags<Enum, N> flags) noexcept {
	bits &= flags.bits;
	return *this;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N>& enum_flags<Enum, N>::operator^=(enum_flags<Enum, N> flags) noexcept {
	bits ^= flags.bits;
	return *this;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N> enum_flags<Enum, N>::operator|(enum_flags<Enum, N> flags) const noexcept {
	auto ret = *this;
	ret |= flags;
	return ret;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N> enum_flags<Enum, N>::operator&(enum_flags<Enum, N> flags) const noexcept {
	auto ret = *this;
	ret &= flags;
	return ret;
}

template <typename Enum, std::size_t N>
constexpr enum_flags<Enum, N> enum_flags<Enum, N>::operator^(enum_flags<Enum, N> flags) const noexcept {
	auto ret = *this;
	ret ^= flags;
	return ret;
}

template <typename Enum, std::size_t N>
constexpr bool operator==(enum_flags<Enum, N> lhs, enum_flags<Enum, N> rhs) noexcept {
	return lhs.bits == rhs.bits;
}

template <typename Enum, std::size_t N>
constexpr bool operator!=(enum_flags<Enum, N> lhs, enum_flags<Enum, N> rhs) noexcept {
	return !(lhs == rhs);
}

} // namespace kt

template <typename Enum, std::size_t N>
constexpr kt::enum_flags<Enum, N> operator|(Enum flag1, Enum flag2) noexcept {
	return kt::enum_flags<Enum, N>(flag1) | kt::enum_flags<Enum, N>(flag2);
}

template <typename Enum, std::size_t N>
constexpr kt::enum_flags<Enum, N> operator&(Enum flag1, Enum flag2) noexcept {
	return kt::enum_flags<Enum, N>(flag1) & kt::enum_flags<Enum, N>(flag2);
}

template <typename Enum, std::size_t N>
constexpr kt::enum_flags<Enum, N> operator^(Enum flag1, Enum flag2) noexcept {
	return kt::enum_flags<Enum, N>(flag1) ^ kt::enum_flags<Enum, N>(flag2);
}
