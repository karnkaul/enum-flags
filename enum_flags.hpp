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
template <typename Enum, Enum Begin = static_cast<Enum>(0), Enum End = Enum::eCOUNT_>
struct enum_flags {
	static_assert(std::is_enum_v<Enum>, "Enum must be an enum!");

	using value_type = Enum;
	using u_type = std::underlying_type_t<Enum>;
	using size_type = std::size_t;
	static constexpr size_type size_v = static_cast<size_type>(static_cast<u_type>(End) - static_cast<u_type>(Begin));

	using storage = std::bitset<size_v>;
	using reference = typename storage::reference;

	storage bits;

	static constexpr enum_flags<Enum, Begin, End> inverse() noexcept;

	constexpr enum_flags() noexcept = default;
	///
	/// \brief Construct via uint representation of flags
	///
	constexpr /*implicit*/ enum_flags(std::uint64_t value) noexcept { bits = value; }
	///
	/// \brief Construct with a single flag set
	///
	constexpr /*implicit*/ enum_flags(Enum flag) noexcept { bits.set((std::size_t)flag); }

	///
	/// \brief Check if passed flags are set
	///
	constexpr bool test(enum_flags<Enum, Begin, End> flags) const noexcept { return (bits & flags.bits) == flags.bits; }
	///
	/// \brief Obtain count of set flags
	///
	constexpr size_type test() const noexcept { return bits.count(); }
	///
	/// \brief Set passed flags
	///
	constexpr enum_flags<Enum, Begin, End>& set(enum_flags<Enum, Begin, End> flags = inverse()) noexcept;
	///
	/// \brief Reset passed flags
	///
	constexpr enum_flags<Enum, Begin, End>& reset(enum_flags<Enum, Begin, End> flags = inverse()) noexcept;
	///
	/// \brief Flip passed flags
	///
	constexpr enum_flags<Enum, Begin, End>& flip(enum_flags<Enum, Begin, End> flags = inverse()) noexcept;
	///
	/// \brief Check if all passed flags are set
	///
	constexpr bool all(enum_flags<Enum, Begin, End> flags = inverse()) const noexcept { return (flags.bits & bits) == flags.bits; }
	///
	/// \brief Check if any passed flags are set
	///
	constexpr bool any(enum_flags<Enum, Begin, End> flags = inverse()) const noexcept { return (flags.bits & bits).any(); }
	///
	/// \brief Check if no passed flags are set
	///
	constexpr bool none(enum_flags<Enum, Begin, End> flags = inverse()) const noexcept { return (flags.bits & bits).none(); }
	///
	/// \brief Obtain count of set flags
	///
	constexpr size_type count(enum_flags<Enum, Begin, End> flags = inverse()) const noexcept { return (flags.bits & bits).count(); }

	///
	/// \brief Obtain a reference to the flag
	///
	constexpr reference operator[](Enum flag) noexcept { return bits[(std::size_t)flag]; }
	///
	/// \brief Perform bitwise `OR` / add flags
	///
	constexpr enum_flags<Enum, Begin, End>& operator|=(enum_flags<Enum, Begin, End> flags) noexcept;
	///
	/// \brief Perform bitwise `AND` / multiply flags
	///
	constexpr enum_flags<Enum, Begin, End>& operator&=(enum_flags<Enum, Begin, End> flags) noexcept;
	///
	/// \brief Perform bitwise `XOR` / exclusively add flags (add mod 2)
	///
	constexpr enum_flags<Enum, Begin, End>& operator^=(enum_flags<Enum, Begin, End> flags) noexcept;
	///
	/// \brief Perform bitwise `OR` / add flags
	///
	constexpr enum_flags<Enum, Begin, End> operator|(enum_flags<Enum, Begin, End> flags) const noexcept;
	///
	/// \brief Perform bitwise `AND` / multiply flags
	///
	constexpr enum_flags<Enum, Begin, End> operator&(enum_flags<Enum, Begin, End> flags) const noexcept;
	///
	/// \brief Perform bitwise `XOR` / exclusively add flags (add mod 2)
	///
	constexpr enum_flags<Enum, Begin, End> operator^(enum_flags<Enum, Begin, End> flags) const noexcept;

	friend constexpr bool operator==(enum_flags<Enum, Begin, End> lhs, enum_flags<Enum, Begin, End> rhs) noexcept { return lhs.bits == rhs.bits; }

	friend constexpr bool operator!=(enum_flags<Enum, Begin, End> lhs, enum_flags<Enum, Begin, End> rhs) noexcept { return !(lhs == rhs); }
};

// impl

template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End> enum_flags<Enum, Begin, End>::inverse() noexcept {
	enum_flags<Enum, Begin, End> ret;
	ret.bits.flip();
	return ret;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::set(enum_flags<Enum, Begin, End> flags) noexcept {
	bits |= flags.bits;
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::reset(enum_flags<Enum, Begin, End> flags) noexcept {
	bits &= flags.bits.flip();
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::flip(enum_flags<Enum, Begin, End> flags) noexcept {
	bits ^= flags.bits;
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::operator|=(enum_flags<Enum, Begin, End> flags) noexcept {
	bits |= flags.bits;
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::operator&=(enum_flags<Enum, Begin, End> flags) noexcept {
	bits &= flags.bits;
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End>& enum_flags<Enum, Begin, End>::operator^=(enum_flags<Enum, Begin, End> flags) noexcept {
	bits ^= flags.bits;
	return *this;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End> enum_flags<Enum, Begin, End>::operator|(enum_flags<Enum, Begin, End> flags) const noexcept {
	auto ret = *this;
	ret |= flags;
	return ret;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End> enum_flags<Enum, Begin, End>::operator&(enum_flags<Enum, Begin, End> flags) const noexcept {
	auto ret = *this;
	ret &= flags;
	return ret;
}
template <typename Enum, Enum Begin, Enum End>
constexpr enum_flags<Enum, Begin, End> enum_flags<Enum, Begin, End>::operator^(enum_flags<Enum, Begin, End> flags) const noexcept {
	auto ret = *this;
	ret ^= flags;
	return ret;
}
} // namespace kt
