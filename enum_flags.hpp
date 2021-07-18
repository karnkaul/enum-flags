// KT header-only library
// Requirements: C++17

#pragma once
#include <cstdint>
#include <type_traits>

namespace kt {
///
/// \brief Trait for linear enums (0, 1, 2, 3, ...)
///
struct enum_trait_linear {};
///
/// \brief Trait for power of two enums (1, 2, 4, 8, ...)
///
struct enum_trait_pot {};

namespace detail {
template <typename T, typename... U>
constexpr bool any_same_v = (std::is_same_v<T, U> || ...);
}

///
/// \brief Wrapper around an integral type used as bit flags
///
template <typename Enum, typename Ty = std::uint32_t, typename Tr = enum_trait_linear>
class enum_flags {
	static_assert(std::is_enum_v<Enum>, "Enum must be an enum");
	static_assert(std::is_integral_v<Ty>, "Ty must be integral");
	static_assert(detail::any_same_v<Tr, enum_trait_linear, enum_trait_pot>, "Invalid enum trait");

  public:
	using type = Enum;
	using storage_t = Ty;
	static constexpr bool is_linear_v = std::is_same_v<Tr, enum_trait_linear>;

	///
	/// \brief Build an instance by setting t
	///
	template <typename... T>
	static constexpr enum_flags make(T... t) noexcept;

	///
	/// \brief Default constructor
	///
	constexpr enum_flags() = default;
	///
	/// \brief Set flag e
	///
	constexpr enum_flags(Enum e) noexcept;
	///
	/// \brief Set inputs
	///
	template <typename... T>
	constexpr enum_flags(T... t) noexcept;
	///
	/// \brief Conversion operator
	///
	constexpr explicit operator Ty() const noexcept { return m_bits; }

	///
	/// \brief Set inputs
	///
	template <typename... T>
	constexpr enum_flags& set(T... t) noexcept;
	///
	/// \brief Remove inputs
	///
	template <typename... T>
	constexpr enum_flags& reset(T... t) noexcept;
	///
	/// \brief Assign value to mask bits
	///
	constexpr enum_flags& assign(enum_flags mask, bool value) noexcept;

	///
	/// \brief Test if e is set
	///
	constexpr bool test(Enum e) const noexcept { return all(e); }
	///
	/// \brief Test if e is set
	///
	constexpr bool operator[](Enum e) const noexcept { return test(e); }
	///
	/// \brief Test if any bits are set
	///
	constexpr bool any() const noexcept { return m_bits != Ty{}; }
	///
	/// \brief Test if any bits in mask are set
	///
	constexpr bool any(enum_flags mask) const noexcept { return (m_bits & mask.m_bits) != Ty{}; }
	///
	/// \brief Test if all bits in mask are set
	///
	constexpr bool all(enum_flags mask) const noexcept { return (m_bits & mask.m_bits) == mask.m_bits; }
	///
	/// \brief Obtain number of set bits
	///
	constexpr std::size_t count() const noexcept;
	///
	/// \brief Add set bits and remove unset bits
	///
	constexpr enum_flags& update(enum_flags set, enum_flags reset = {}) noexcept;

	///
	/// \brief Compare two enum_flags
	///
	friend constexpr bool operator==(enum_flags a, enum_flags b) noexcept { return a.m_bits == b.m_bits; }
	///
	/// \brief Compare two enum_flags
	///
	friend constexpr bool operator!=(enum_flags a, enum_flags b) noexcept { return !(a == b); }

	///
	/// \brief Perform bitwise OR / add flags
	///
	constexpr enum_flags& operator|=(enum_flags mask) noexcept { return (m_bits |= mask.m_bits, *this); }
	///
	/// \brief Perform bitwise AND / multiply flags
	///
	constexpr enum_flags& operator&=(enum_flags mask) noexcept { return (m_bits &= mask.m_bits, *this); }
	///
	/// \brief Perform bitwise XOR / exclusively add flags (add mod 2)
	///
	constexpr enum_flags& operator^=(enum_flags mask) noexcept { return (m_bits ^= mask.m_bits, *this); }

	///
	/// \brief Perform bitwise OR / add flags
	///
	friend constexpr enum_flags operator|(enum_flags lhs, enum_flags rhs) noexcept { return lhs |= rhs; }
	///
	/// \brief Perform bitwise AND / multiply flags
	///
	friend constexpr enum_flags operator&(enum_flags lhs, enum_flags rhs) noexcept { return lhs &= rhs; }
	///
	/// \brief Perform bitwise XOR / exclusively add flags (add mod 2)
	///
	friend constexpr enum_flags operator^(enum_flags lhs, enum_flags rhs) noexcept { return lhs ^= rhs; }

  private:
	Ty m_bits{};
};

// impl
} // namespace kt

namespace kt {
namespace detail {
template <typename Enum, typename Ty, typename Tr, typename T>
constexpr bool enum_flags_valid_arg = std::is_same_v<T, Enum> || std::is_same_v<T, enum_flags<Enum, Ty, Tr>>;
}

template <typename Enum, typename Ty, typename Tr>
constexpr enum_flags<Enum, Ty, Tr>::enum_flags(Enum e) noexcept {
	if constexpr (is_linear_v) {
		m_bits |= (1 << static_cast<Ty>(e));
	} else {
		m_bits |= static_cast<Ty>(e);
	}
}
template <typename Enum, typename Ty, typename Tr>
template <typename... T>
constexpr enum_flags<Enum, Ty, Tr>::enum_flags(T... t) noexcept {
	set(t...);
}
template <typename Enum, typename Ty, typename Tr>
template <typename... T>
constexpr enum_flags<Enum, Ty, Tr> enum_flags<Enum, Ty, Tr>::make(T... t) noexcept {
	static_assert((detail::enum_flags_valid_arg<Enum, Ty, Tr, T> && ...), "Invalid T");
	enum_flags ret{};
	ret.set(t...);
	return ret;
}
template <typename Enum, typename Ty, typename Tr>
template <typename... T>
constexpr enum_flags<Enum, Ty, Tr>& enum_flags<Enum, Ty, Tr>::set(T... t) noexcept {
	static_assert((detail::enum_flags_valid_arg<Enum, Ty, Tr, T> && ...), "Invalid T");
	(update(t), ...);
	return *this;
}
template <typename Enum, typename Ty, typename Tr>
template <typename... T>
constexpr enum_flags<Enum, Ty, Tr>& enum_flags<Enum, Ty, Tr>::reset(T... t) noexcept {
	static_assert((detail::enum_flags_valid_arg<Enum, Ty, Tr, T> && ...), "Invalid T");
	(update({}, t), ...);
	return *this;
}
template <typename Enum, typename Ty, typename Tr>
constexpr enum_flags<Enum, Ty, Tr>& enum_flags<Enum, Ty, Tr>::assign(enum_flags mask, bool value) noexcept {
	if (value) {
		set(mask);
	} else {
		reset(mask);
	}
	return *this;
}
template <typename Enum, typename Ty, typename Tr>
constexpr enum_flags<Enum, Ty, Tr>& enum_flags<Enum, Ty, Tr>::update(enum_flags set, enum_flags unset) noexcept {
	m_bits |= set.m_bits;
	m_bits &= ~unset.m_bits;
	return *this;
}

template <typename Enum, typename Ty, typename Tr>
constexpr std::size_t enum_flags<Enum, Ty, Tr>::count() const noexcept {
	std::size_t ret{};
	Ty bit = static_cast<Ty>(1);
	for (std::size_t i = 0; i < sizeof(Ty) * 8; ++i) {
		if ((m_bits & bit) != 0) { ++ret; }
		bit <<= 1;
	}
	return ret;
}
} // namespace kt
